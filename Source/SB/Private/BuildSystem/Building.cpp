#include "BuildSystem/Building.h"
#include "BuildSystem/BuildSystem.h"
#include "BuildSystem/CentralHubBuilding.h"
#include "BuildSystem/PowerFacility.h"
#include "Components/BoxComponent.h"
#include "SB/DebugMacro.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;
	BuildBlocker = CreateDefaultSubobject<UBoxComponent>("BuildBlocker");
	BuildBlocker->SetCollisionProfileName("BuildBlocker");
	SetRootComponent(BuildBlocker);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName("Building");
	Mesh->SetupAttachment(BuildBlocker);
}


void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SCREEN_LOG_SINGLE_FRAME(FString::FromInt(PowerConsumption));
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	SnapLocation(GetActorLocation());
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
	if (ParentBuilding && !bPowerOn)
	{
		ParentBuilding->DecreasePowerConsumption(Power);
	}
}

void ABuilding::PropagatePowerState()
{
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
	Mesh->SetVisibility(bVisibility);
	SetActorLocation(FVector(0.0f, 0.0f, 100000.0f));
}

void ABuilding::SetAsPreview()
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BuildBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->bReceivesDecals = false;
	SetActorRelativeLocation(FVector(0.0f, 0.0f, GetZOffset()));
	
}

void ABuilding::SetAllMaterials(UMaterialInterface* Material)
{
	if (Material)
	{
		int MaterialCounts = Mesh->GetNumMaterials();
		for (int i = 0; i < MaterialCounts; ++i)
		{
			Mesh->SetMaterial(i, Material);
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
		return ParentBuilding->IsOperating();
	}
}

void ABuilding::SnapLocation(FVector WorldLocation)
{
	SetActorLocation(BuildSystem::SnapLocationXY(WorldLocation));
}

void ABuilding::SetOutlineDraw(bool bDraw, int Color)
{
	SetRenderCustomDepthStencil(bDraw, Color);
}

void ABuilding::SetRenderCustomDepthStencil(bool bRender, int Stencil)
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(bRender);
		Mesh->SetCustomDepthStencilValue(Stencil);
	}
}
