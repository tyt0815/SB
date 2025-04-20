#include "BuildSystem/BuildingCreater.h"
#include "BuildSystem/Building.h"
#include "Components/ArrowComponent.h"
#include "Components/DecalComponent.h"
#include "BuildSystem/BuildSystemDefines.h"

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
			Location.X = CELL_SIZE * (i - CellExtentX);
			Location.Y = CELL_SIZE * (j - CellExtentY);
			Location.Z = 0.0f;
			DecalComponents[i][j]->SetRelativeLocation(Location);
			DecalComponents[i][j]->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
			DecalComponents[i][j]->DecalSize = FVector(50.0f, CELL_SIZE / 2.0f, CELL_SIZE / 2.0f);
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

void ABuildingCreater::SetPreviewBuilding(TSubclassOf<ABuilding> BuildingClass)
{
	if (BuildingClass)
	{
		DestroyPreviewBuilding();

		PreviewBuildingClass = BuildingClass;
		PreviewBuilding = GetWorld()->SpawnActor<ABuilding>(PreviewBuildingClass);
		if (PreviewBuilding)
		{
			PreviewBuilding->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PreviewBuilding->SetAsPreview(PreviewMaterial);
			HiddenInGame(false);
		}
	}
}

void ABuildingCreater::CreateBuilding()
{
	if (PreviewBuilding && PreviewBuildingClass)
	{
		FActorSpawnParameters SpawnParameters;
		GetWorld()->SpawnActor<ABuilding>(PreviewBuildingClass, PreviewBuilding->GetTransform());
	}
	DestroyPreviewBuilding();
	HiddenInGame(true);
}

void ABuildingCreater::DestroyPreviewBuilding()
{
	if (PreviewBuilding)
	{
		PreviewBuilding->Destroy();
	}
	PreviewBuilding = nullptr;
	PreviewBuildingClass = nullptr;
}

void ABuildingCreater::HiddenInGame(bool bNew)
{
	SetGridVisibility(!bNew);
	for (auto& Rows : DecalComponents)
	{
		for (auto& Decal : Rows)
		{
			if (Decal)
			{
				Decal->bHiddenInGame = bNew;
			}
		}
	}
}

void ABuildingCreater::SnapLocation(FVector WorldLocation)
{
	int XIndex = WorldLocation.X / CELL_SIZE;
	int YIndex = WorldLocation.Y / CELL_SIZE;
	FVector SnappedLocation = WorldLocation;
	SnappedLocation.X = CELL_SIZE * XIndex;
	SnappedLocation.Y = CELL_SIZE * YIndex;
	if (WorldLocation.X >= 0)
	{
		SnappedLocation.X += CELL_SIZE * 0.5f;
	}
	else
	{
		SnappedLocation.X -= CELL_SIZE * 0.5f;
	}
	if (WorldLocation.Y >= 0)
	{
		SnappedLocation.Y += CELL_SIZE * 0.5f;
	}
	else
	{
		SnappedLocation.Y -= CELL_SIZE * 0.5f;
	}
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
