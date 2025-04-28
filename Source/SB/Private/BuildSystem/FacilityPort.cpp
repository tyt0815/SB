#include "BuildSystem/FacilityPort.h"
#include "Components/StaticMeshComponent.h"

AFacilityPort::AFacilityPort()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
}

void AFacilityPort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateToken();
}

void AFacilityPort::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFacilityPort::UpdateToken()
{
	if (Token > 0)
	{
		Token = (Token + 1) % 3;
	}
}

