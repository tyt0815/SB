#include "BuildSystem/BuildingCreater.h"
#include "BuildSystem/Building.h"
#include "BuildSystem/BuildSystem.h"
#include "Components/ArrowComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ABuildingCreater::ABuildingCreater()
{
	PrimaryActorTick.bCanEverTick = true;
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Root"));
	SetRootComponent(ArrowComponent);

	DecalComponents.SetNum(1 + 2 * CellExtentX);
	bValidCell.SetNum(1 + 2 * CellExtentX);
	for (int i = 0; i <= 2 * CellExtentX; ++i)
	{
		DecalComponents[i].SetNum(1 + 2 * CellExtentY);
		bValidCell[i].SetNum(1 + 2 * CellExtentY);
		for (int j = 0; j <= 2 * CellExtentY; ++j)
		{
			DecalComponents[i][j] = CreateDefaultSubobject<UDecalComponent>(FName(FString::Printf(TEXT("Decal_%d_%d"), i, j)));
			DecalComponents[i][j]->SetupAttachment(GetRootComponent());
			DecalComponents[i][j]->SetRelativeLocation(GetCellLocation(i, j));
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
			PreviewBuilding->SetAsPreview();
			UpdateBuildableStateAndSetPreviewBuildingMaterial();
			HiddenInGame(false);
		}
	}
}

void ABuildingCreater::CreateBuilding()
{
	if (bBuildable)
	{
		if (PreviewBuilding && PreviewBuildingClass)
		{
			ABuilding* Building = GetWorld()->SpawnActor<ABuilding>(PreviewBuildingClass);
			if (Building)
			{
				FVector Location = PreviewBuilding->GetActorLocation();
				CancelPreview();
				Building->Place(Location);
			}
		}
	}
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
	FVector SnappedLocation = BuildSystem::SnapLocationXY(WorldLocation);
	
	FVector CurrLocation = GetActorLocation();
	if (CurrLocation != SnappedLocation)
	{
		SetActorLocation(SnappedLocation);
		UpdateValidCells();
		UpdateBuildableStateAndSetPreviewBuildingMaterial();
	}
}

void ABuildingCreater::CancelPreview()
{
	DestroyPreviewBuilding();
	HiddenInGame(true);
}

void ABuildingCreater::RotatePreviewBuildingClockwise90()
{
	if (PreviewBuilding)
	{
		PreviewBuilding->RotateClockwise90();
	}
}

FVector ABuildingCreater::GetCellLocation(int i, int j)
{
	FVector Location;
	Location.X = CELL_SIZE * (i - CellExtentX);
	Location.Y = CELL_SIZE * (j - CellExtentY);
	Location.Z = 0.0f;
	return Location;
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

void ABuildingCreater::UpdateValidCells()
{
	// TODO
	for (int i = 0; i < bValidCell.Num(); ++i)
	{
		for (int j = 0; j < bValidCell[i].Num(); ++j)
		{
			bValidCell[i][j] = true;
		}
	}
}

void ABuildingCreater::UpdateBuildableState()
{
	if (PreviewBuilding)
	{
		FIntVector Extent = PreviewBuilding->GetCellExtent();
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		ActorsToIgnore.Add(PreviewBuilding);
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
		for (int i = -Extent.X + 1; i < Extent.X; ++i)
		{
			for (int j = -Extent.Y + 1; j < Extent.Y; ++j)
			{
				FVector Location = PreviewBuilding->GetActorLocation();
				Location.X += i * CELL_SIZE;
				Location.Y += j * CELL_SIZE;
				FVector Top = Location + FVector::ZAxisVector * PreviewBuilding->GetHalfHeight();
				FVector Bottom = Location - FVector::ZAxisVector * PreviewBuilding->GetHalfHeight();
				FHitResult HitResult;
				UKismetSystemLibrary::LineTraceSingleForObjects(
					this,
					Top,
					Bottom,
					ObjectTypes,
					false,
					ActorsToIgnore,
					EDrawDebugTrace::None,
					HitResult,
					true
				);
				if (HitResult.GetActor())
				{
					bBuildable = false;
					return;
				}
			}
		}
	}
	bBuildable = true;
}

void ABuildingCreater::SetPreviewBuildingMaterial(UMaterialInterface* Material)
{
	if (PreviewBuilding && Material)
	{
		PreviewBuilding->SetAllMaterials(Material);
	}
}

void ABuildingCreater::UpdateBuildableStateAndSetPreviewBuildingMaterial()
{
	UpdateBuildableState();
	if (bBuildable)
	{
		SetPreviewBuildingMaterial(ValidPreviewMaterial);
	}
	else
	{
		SetPreviewBuildingMaterial(InvalidPreviewMaterial);
	}
	
}
