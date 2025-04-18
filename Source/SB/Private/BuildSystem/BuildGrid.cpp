#include "BuildSystem/BuildGrid.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"

ABuildGrid::ABuildGrid()
{
	PrimaryActorTick.bCanEverTick = true;

	BorderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Border"));
	SetRootComponent(BorderMesh);
	BorderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BorderMesh->CastShadow = false;
	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grid"));
	GridMesh->SetupAttachment(BorderMesh);
	GridMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GridMesh->CastShadow = false;
}

void ABuildGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildGrid::BeginPlay()
{
	Super::BeginPlay();
	
}