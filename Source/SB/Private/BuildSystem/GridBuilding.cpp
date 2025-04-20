#include "BuildSystem/GridBuilding.h"
#include "BuildSystem/BuildSystem.h"
#include "Components/BoxComponent.h"

AGridBuilding::AGridBuilding()
{
	Grid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grid"));
	Grid->SetupAttachment(BuildBlocker);
	Grid->SetCollisionProfileName("NoCollision");
	GridBoundary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridBoundary"));
	GridBoundary->SetupAttachment(BuildBlocker);
	GridBoundary->SetCollisionProfileName("GridBoundary");
}

void AGridBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridBuilding::BeginPlay()
{
	Super::BeginPlay();
}

void AGridBuilding::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Grid->SetRelativeLocation(FVector(0.0f, 0.0f, 1 + CELL_SIZE * (0.5f - CellExtent.Z)));
	GridBoundary->SetRelativeLocation(FVector(0.0f, 0.0f, CELL_SIZE * (1 - CellExtent.Z)));
	FVector Scale;
	Scale.X = 2 * GridExtent.X - 1;
	Scale.Y = 2 * GridExtent.Y - 1;
	Scale.Z = 1.0f;
	Grid->SetRelativeScale3D(Scale);
	GridBoundary->SetRelativeScale3D(Scale);
}
