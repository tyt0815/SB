#include "BuildSystem/GridBuilding.h"
#include "BuildSystem/BuildSystem.h"
#include "Components/BoxComponent.h"
#include "Characters/Player/SBPlayer.h"
#include "SB/DebugMacro.h"

AGridBuilding::AGridBuilding()
{
	GridBoundaryBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GridBoundaryBox"));
	GridBoundaryBox->SetupAttachment(BuildBlocker);
	GridBoundaryBox->SetCollisionProfileName("GridBoundary");
	GridBoundaryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridBoundaryMesh"));
	GridBoundaryMesh->SetupAttachment(BuildBlocker);
	GridBoundaryMesh->SetCollisionProfileName("NoCollision");
}

void AGridBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridBuilding::BeginPlay()
{
	Super::BeginPlay();

	GridBoundaryBox->OnComponentBeginOverlap.AddDynamic(this, &AGridBuilding::OnPlayerBeginOverlapGridBoundary);
	GridBoundaryBox->OnComponentEndOverlap.AddDynamic(this, &AGridBuilding::OnPlayerEndOverlapGridBoundary);

	TSet<AActor*> OverlappingActors;
	GridBoundaryBox->GetOverlappingActors(OverlappingActors);
	OverlappingActors.Remove(this);
	for (AActor* OverlappingActor : OverlappingActors)
	{
		OnPlayerBeginOverlapGridBoundary(nullptr, OverlappingActor, nullptr, 0, false, FHitResult());
	}
}

void AGridBuilding::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GridBoundaryBox->SetBoxExtent(BuildSystem::CalculateBoxExtent(GridExtent.X, GridExtent.Y, CellExtent.Z));

	GridBoundaryMesh->SetRelativeLocation(FVector(0.0f, 0.0f, CELL_SIZE * (1 - CellExtent.Z)));
	FVector Scale;
	Scale.X = 2 * GridExtent.X - 1;
	Scale.Y = 2 * GridExtent.Y - 1;
	Scale.Z = 1.0f;
	GridBoundaryMesh->SetRelativeScale3D(Scale + FVector(0.001f, 0.001f, 0.0f));
}

void AGridBuilding::OnPlayerBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SCREEN_LOG_NONE_KEY(OtherActor->GetName());
}

void AGridBuilding::OnPlayerEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SCREEN_LOG_NONE_KEY(OtherActor->GetName());
}
