#include "BuildSystem/Building.h"
#include "BuildSystem/BuildSystem.h"
#include "BuildSystem/CentralHubBuilding.h"
#include "BuildSystem/PowerFacility.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SB/DebugMacro.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;
	BuildBlocker = CreateDefaultSubobject<UBoxComponent>("BuildBlocker");
	BuildBlocker->SetCollisionProfileName("BuildBlocker");
	SetRootComponent(BuildBlocker);
	StaticMesh->SetCollisionProfileName("Building");
	StaticMesh->SetupAttachment(BuildBlocker);
	SkeletalMesh->SetCollisionProfileName("Building");
	SkeletalMesh->SetupAttachment(BuildBlocker);
	bStackable = false;
	bSimulatePhysics = false;
}


void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ABuilding::IncreasePowerConsumption(uint16 Power)
{
	PowerConsumption += Power;
	if (ParentBuilding && bPowerOn)
	{
		ParentBuilding->IncreasePowerConsumption(Power);
	}
}

void ABuilding::DecreasePowerConsumption(uint16 Power)
{
	PowerConsumption -= Power;
	if (ParentBuilding && bPowerOn)
	{
		ParentBuilding->DecreasePowerConsumption(Power);
	}
}

void ABuilding::PropagatePowerState()
{
}

void ABuilding::Place(FVector WorldLocation, FRotator WorldRotation)
{
	SnapLocationXY(WorldLocation);
	SetActorRotation(WorldRotation);
	TryConnectToNearByFacility();
}

void ABuilding::TryConnectToNearByFacility()
{
	TArray<FHitResult> HitResults;
	TraceGridBuilding(HitResults);
	if (BuildingType == EBuildingType::EBT_HubLinkedFacility)
	{
		for (const auto& HitResult : HitResults)
		{
			if (HitResult.GetActor())
			{
				ACentralHubBuilding* Hub = Cast<ACentralHubBuilding>(HitResult.GetActor());
				if (Hub)
				{
					TryConnectToBuilding(Hub);
					return;
				}
			}
		}
	}
	else if (BuildingType == EBuildingType::EBT_PowerLinkedFacility)
	{
		for (const auto& HitResult : HitResults)
		{
			if (HitResult.GetActor())
			{
				APowerFacility* PowerFacility = Cast<APowerFacility>(HitResult.GetActor());
				if (PowerFacility)
				{
					TryConnectToBuilding(PowerFacility);
					return;
				}
			}
		}
	}
}

void ABuilding::TraceGridBuilding(TArray<FHitResult>& HitResults)
{
	FVector Start = GetActorLocation() + (FVector::ZAxisVector * BuildBlocker->GetScaledBoxExtent().Z);
	FVector End = GetActorLocation() - (FVector::ZAxisVector * BuildBlocker->GetScaledBoxExtent().Z);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this,
		Start,
		End,
		FVector(BuildBlocker->GetScaledBoxExtent().X, BuildBlocker->GetScaledBoxExtent().Y, 0.0f),
		FRotator(),
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
}

void ABuilding::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BuildBlocker->SetBoxExtent(BuildSystem::CalculateBoxExtent(CellExtent.X, CellExtent.Y, CellExtent.Z));
}

void ABuilding::OnMouseHoverStarted()
{
	if (!bSelected)
	{
		SetOutlineDraw(true, 1);
	}
}

void ABuilding::OnMouseHoverEnded()
{
	if (!bSelected)
	{
		SetOutlineDraw(false, 0);
	}
}

void ABuilding::OnSelected()
{
	bSelected = true;
	SetOutlineDraw(true, 2);
}

void ABuilding::OnDeselected()
{
	bSelected = false;
	SetOutlineDraw(false, 0);
}

void ABuilding::OnConnectToBuilding(ABuilding* Parent)
{
	if (Parent && Parent != ParentBuilding)
	{
		TryDisconnectToBuilding();
		if (bPowerOn)
		{
			Parent->IncreasePowerConsumption(PowerConsumption);
		}
		ParentBuilding = Parent;
	}
}

void ABuilding::OnDisconnectToBuilding()
{
	if (ParentBuilding)
	{
		if (bPowerOn)
		{
			ParentBuilding->DecreasePowerConsumption(PowerConsumption);
		}
	}
	ParentBuilding = nullptr;
}

void ABuilding::TryConnectToBuilding(ABuilding* Parent)
{
	switch (BuildingType)
	{
	case EBuildingType::EBT_StandAlone:
		break;
	case EBuildingType::EBT_HubLinkedFacility:
	{
		ACentralHubBuilding* Hub = Cast<ACentralHubBuilding>(Parent);
		if (Hub)
		{
			Hub->ConnectToBuilding(this);
		}
		break;
	}
	case EBuildingType::EBT_PowerLinkedFacility:
	{
		APowerFacility* PowerFacility = Cast<APowerFacility>(Parent);
		if (PowerFacility)
		{
			PowerFacility->ConnectToBuilding(this);
		}
	}
		break;
	default:
		break;
	}
}

void ABuilding::TryDisconnectToBuilding()
{
	switch (BuildingType)
	{
	case EBuildingType::EBT_StandAlone:
		break;
	case EBuildingType::EBT_HubLinkedFacility:
	{
		ACentralHubBuilding* Hub = Cast<ACentralHubBuilding>(ParentBuilding);
		if (Hub)
		{
			Hub->DisconnectToBuilding(this);
		}
		break;
	}
	case EBuildingType::EBT_PowerLinkedFacility:
	{
		APowerFacility* PowerFacility = Cast<APowerFacility>(ParentBuilding);
		if (PowerFacility)
		{
			PowerFacility->DisconnectToBuilding(this);
		}
	}
	break;
	default:
		break;
	}
}

float ABuilding::GetZOffset() const
{
	return BuildBlocker->GetScaledBoxExtent().Z;
}

float ABuilding::GetHalfWidth() const
{
	return BuildBlocker->GetScaledBoxExtent().X;
}

float ABuilding::GetHalfDepth() const
{
	return BuildBlocker->GetScaledBoxExtent().Y;
}

float ABuilding::GetHalfHeight() const
{
	return BuildBlocker->GetScaledBoxExtent().Z;
}

float ABuilding::GetWidth() const
{
	return GetHalfWidth() * 2;
}

float ABuilding::GetDepth() const
{
	return GetHalfDepth() * 2;
}

float ABuilding::GetHeight() const
{
	return GetHalfHeight() * 2;
}

void ABuilding::SetVisibility(bool bVisibility)
{
	if (StaticMesh->GetStaticMesh())
	{
		StaticMesh->SetVisibility(bVisibility);
	}
	if (SkeletalMesh->GetSkeletalMeshAsset())
	{
		SkeletalMesh->SetVisibility(bVisibility);
	}
}

void ABuilding::SetAsPreview()
{
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BuildBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->bReceivesDecals = false;
	SkeletalMesh->bReceivesDecals = false;
	StaticMesh->SetSimulatePhysics(false);
	SkeletalMesh->SetSimulatePhysics(false);
	TArray<AActor*> ChildActors;
	GetAllChildActors(ChildActors, true);
	for (AActor* Child : ChildActors)
	{
		ABuilding* Building = Cast<ABuilding>(Child);
		if (Building)
		{
			Building->SetAsPreview();
		}
	}
	bPreview = true;
}

void ABuilding::SetAllMaterials(UMaterialInterface* Material)
{
	if (Material)
	{
		int MaterialCounts = StaticMesh->GetNumMaterials();
		for (int i = 0; i < MaterialCounts; ++i)
		{
			StaticMesh->SetMaterial(i, Material);
		}
		MaterialCounts = SkeletalMesh->GetNumMaterials();
		for (int i = 0; i < MaterialCounts; ++i)
		{
			SkeletalMesh->SetMaterial(i, Material);
		}
	}
}

void ABuilding::SetPowerState(bool bOn)
{
	if (bPowerOn != bOn)
	{
		bPowerOn = bOn;

		if (ParentBuilding)
		{
			if (bPowerOn)
			{
				ParentBuilding->IncreasePowerConsumption(PowerConsumption);
			}
			else
			{
				ParentBuilding->DecreasePowerConsumption(PowerConsumption);
			}

		}
	}
}

bool ABuilding::IsOperating() const
{
	return IsPowerOn() && HasSufficientPower();
}

bool ABuilding::IsPowerOn() const
{
	return bPowerOn;
}

bool ABuilding::HasSufficientPower() const
{
	if (BuildingType == EBuildingType::EBT_StandAlone)
	{
		return true;
	}
	else
	{
		return IsConnectedToParentBuilding() && ParentBuilding->IsOperating();
	}
}

void ABuilding::RotateClockwise90()
{
	AddActorWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void ABuilding::SnapLocationXY(FVector WorldLocation)
{
	SetActorLocation(BuildSystem::SnapLocationXY(WorldLocation));
}

void ABuilding::InitMeshsVisibilityAndPhysics()
{
}

void ABuilding::BeginDestroy()
{
	TryDisconnectToBuilding();
	Super::BeginDestroy();
}

void ABuilding::OnPickedUp(AActor* OtherActor)
{
	TryDisconnectToBuilding();
	Destroy();
}

void ABuilding::TraceBuilding(FVector Start, FVector End, FHitResult& HitResult)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FVector BoxExtent = BuildBlocker->GetScaledBoxExtent();
	BoxExtent.X = 0.0f;
	FRotator Orient = (End - Start).Rotation();

	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		Start,
		End,
		BoxExtent,
		Orient,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);
}

void ABuilding::TraceBuildings(FVector Start, FVector End, TArray<FHitResult>& HitResults)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::LineTraceMultiForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
}

void ABuilding::SetOutlineDraw(bool bDraw, int Color)
{
	SetRenderCustomDepthStencil(bDraw, Color);
}

void ABuilding::SetRenderCustomDepthStencil(bool bRender, int Stencil)
{
	if (StaticMesh->GetStaticMesh())
	{
		StaticMesh->SetRenderCustomDepth(bRender);
		StaticMesh->SetCustomDepthStencilValue(Stencil);
	}
	if (SkeletalMesh->GetSkeletalMeshAsset())
	{
		SkeletalMesh->SetRenderCustomDepth(bRender);
		SkeletalMesh->SetCustomDepthStencilValue(Stencil);
	}
}
