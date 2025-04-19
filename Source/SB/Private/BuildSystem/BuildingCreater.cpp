#include "BuildSystem/BuildingCreater.h"
#include "BuildSystem/Building.h"
#include "Components/ArrowComponent.h"
#include "Components/DecalComponent.h"

ABuildingCreater::ABuildingCreater()
{
	PrimaryActorTick.bCanEverTick = true;
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Root"));
	SetRootComponent(ArrowComponent);

	DecalComponents.SetNum(1 + 2 * CellExtentX);
	for (int i = 0; i <= 2 * CellExtentX; ++i)
	{
		DecalComponents[i].SetNum(1 + 2 * CellExtentY);
		for (int j = 0; j <= 2 * CellExtentY; ++j)
		{
			DecalComponents[i][j] = CreateDefaultSubobject<UDecalComponent>(FName(FString::Printf(TEXT("Decal_%d_%d"), i, j)));
			DecalComponents[i][j]->SetupAttachment(GetRootComponent());
			FVector Location;
			Location.X = CellSize * (i - CellExtentX);
			Location.Y = CellSize * (j - CellExtentY);
			Location.Z = 0.0f;
			DecalComponents[i][j]->SetRelativeLocation(Location);
			DecalComponents[i][j]->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
			DecalComponents[i][j]->DecalSize = FVector(50.0f, CellSize / 2.0f, CellSize / 2.0f);
		}
	}

	SetGridVisibility(false ,true);
}

void ABuildingCreater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingCreater::BeginPlay()
{
	Super::BeginPlay();

	SetGridMaterial(ValidCellMaterial);
}

void ABuildingCreater::SetPreviewBuilding(uint32 i)
{
	if (BuildingClasses.IsValidIndex(i) && BuildingClasses[i])
	{
		DestroyPreviewBuilding();

		PreviewBuilding = GetWorld()->SpawnActor<ABuilding>(BuildingClasses[i]);
		if (PreviewBuilding)
		{
			PreviewBuilding->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PreviewBuilding->SetAsPreview(PreviewMaterial);
			SetGridVisibility(true);
		}
	}
}

void ABuildingCreater::DestroyPreviewBuilding()
{
	if (PreviewBuilding)
	{
		PreviewBuilding->Destroy();
		PreviewBuilding = nullptr;
	}
}

void ABuildingCreater::SnapLocation(FVector WorldLocation)
{
	int XIndex = WorldLocation.X / CellSize;
	int YIndex = WorldLocation.Y / CellSize;
	FVector SnappedLocation = WorldLocation;
	SnappedLocation.X = CellSize * (XIndex + 0.5f);
	SnappedLocation.Y = CellSize * (YIndex + 0.5f);
	SetActorLocation(SnappedLocation);
}

void ABuildingCreater::SetGridVisibility(bool bVisibility, bool bForce)
{
	if (bVisibility != bGridVisibility || bForce)
	{
		bGridVisibility = bVisibility;
		for (auto& Row : DecalComponents)
		{
			for (auto& Decal : Row)
			{
				Decal->SetVisibility(bVisibility);
			}
		}
	}
}

void ABuildingCreater::SetGridMaterial(UMaterialInterface* Material)
{
	for (auto& Row : DecalComponents)
	{
		for (auto& Decal : Row)
		{
			Decal->SetDecalMaterial(Material);
		}
	}
}
