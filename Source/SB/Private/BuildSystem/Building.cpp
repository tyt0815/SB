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

}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ABuilding::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InitializeBuildBlocker();
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

void ABuilding::SnapLocation(FVector WorldLocation)
{
	SetActorLocation(BuildSystem::SnapLocationXY(WorldLocation));
}

void ABuilding::InitializeBuildBlocker()
{
	int Offset = (CELL_SIZE / 2);
	FVector Extent;
	Extent.X = CELL_SIZE * CellExtent.X - Offset;
	Extent.Y = CELL_SIZE * CellExtent.Y - Offset;
	Extent.Z = CELL_SIZE * CellExtent.Z - Offset;
	BuildBlocker->SetBoxExtent(Extent);
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
