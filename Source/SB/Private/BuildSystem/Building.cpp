#include "BuildSystem/Building.h"
#include "BuildSystem/BuildSystem.h"
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

	SCREEN_LOG_SINGLE_FRAME(FString::FromInt(IsOperating()));
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	SnapLocation(GetActorLocation());
}

void ABuilding::ConnectToHub(ACentralHubBuilding* Hub)
{
	if (Hub)
	{
		LinkedHubs.AddUnique(Hub);
	}
}

void ABuilding::ConnectToPowerFacility(APowerFacility* PowerFacility)
{
	if (PowerFacility)
	{
		LinkedPowerFacilitys.AddUnique(PowerFacility);
	}
}

void ABuilding::DisconnectToHub(ACentralHubBuilding* Hub)
{
	if (Hub)
	{
		LinkedHubs.Remove(Hub);
	}
}

void ABuilding::DisconnectToPowerFacility(APowerFacility* PowerFacility)
{
	if (PowerFacility)
	{
		LinkedPowerFacilitys.Remove(PowerFacility);
	}
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

bool ABuilding::IsOperating() const
{
	switch (BuildingType)
	{
	case EBuildingType::EBT_StandAlone:
		return true;
	case EBuildingType::EBT_HubLinkedFacility:
		if (LinkedHubs.Num() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	case EBuildingType::EBT_PowerLinkedFacility:
		if (LinkedPowerFacilitys.Num() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
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
