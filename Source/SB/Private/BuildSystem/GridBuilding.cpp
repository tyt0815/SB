#include "BuildSystem/GridBuilding.h"
#include "BuildSystem/BuildSystem.h"
#include "Components/BoxComponent.h"
#include "Characters/Player/SBPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
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

	// GridBoundaryBox->OnComponentBeginOverlap.AddDynamic(this, &AGridBuilding::OnBeginOverlapGridBoundary);
	// GridBoundaryBox->OnComponentEndOverlap.AddDynamic(this, &AGridBuilding::OnEndOverlapGridBoundary);
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

void AGridBuilding::SetAsPreview()
{
	Super::SetAsPreview();
	GridBoundaryBox->SetGenerateOverlapEvents(false);
}

void AGridBuilding::TraceBuildingsInBoundary(TArray<FHitResult>& HitResults)
{
	FVector Start = GetActorLocation() + FVector::ZAxisVector * GridBoundaryBox->GetScaledBoxExtent().Z;
	FVector End = GetActorLocation() - FVector::ZAxisVector * GridBoundaryBox->GetScaledBoxExtent().Z;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));	// BuildBlocker
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this,
		Start,
		End,
		FVector(GridBoundaryBox->GetScaledBoxExtent().X, GridBoundaryBox->GetScaledBoxExtent().Y, 0.0f),
		FRotator(),
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		false
	);
}

void AGridBuilding::OnBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AGridBuilding::OnEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
