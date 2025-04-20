#include "BuildSystem/Building.h"
#include "BuildSystem/BuildSystemDefines.h"
#include "Components/BoxComponent.h"
#include "SB/DebugMacro.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionObjectType(UEngineTypes::ConvertToCollisionChannel(EObjectTypeQuery::ObjectTypeQuery1));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}


void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
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
	return BoxComponent->GetScaledBoxExtent().Z;
}

void ABuilding::SetVisibility(bool bVisibility)
{
	Mesh->SetVisibility(bVisibility);
	SetActorLocation(FVector(0.0f, 0.0f, 100000.0f));
}

void ABuilding::SetAsPreview(UMaterialInterface* Material)
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->bReceivesDecals = false;
	SetActorRelativeLocation(FVector(0.0f, 0.0f, GetZOffset()));
	if (Material)
	{
		int MaterialCounts = Mesh->GetNumMaterials();
		for (int i = 0; i < MaterialCounts; ++i)
		{
			Mesh->SetMaterial(i, Material);
		}
	}
}

void ABuilding::SetBoxComponentExtents()
{
	int Offset = (CELL_SIZE / 2);
	FVector Extent;
	Extent.X = CELL_SIZE * CellExtentX - Offset;
	Extent.Y = CELL_SIZE * CellExtentY - Offset;
	Extent.Z = CELL_SIZE * CellExtentZ - Offset;
	BoxComponent->SetBoxExtent(Extent);
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
