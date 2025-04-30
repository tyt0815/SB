#include "BuildSystem/CentralHubBuilding.h"
#include "BuildSystem/PowerFacility.h"
#include "BuildSystem/BuildSystem.h"
#include "BuildSystem/InputPort.h"
#include "BuildSystem/OutputPort.h"
#include "Components/InventoryComponent.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACentralHubBuilding::ACentralHubBuilding()
{
	StorageComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Storage"));
	StorageComponent->SetInventorySize(100.0f);
	StorageComponent->ComponentTags.Add("Storage");
}

void ACentralHubBuilding::Tick(float Delta)
{
	Super::Tick(Delta);
	if (PrevPowerCapacity != PowerCapacity || PrevPowerConsumption != PowerConsumption)
	{
		PrevPowerCapacity = PowerCapacity;
		PrevPowerConsumption = PowerConsumption;
		PropagatePowerState();
	}
}

void ACentralHubBuilding::PropagatePowerState()
{
	for (ABuilding* Building : ChildBuildings)
	{
		Building->PropagatePowerState();
	}
}

bool ACentralHubBuilding::IsOperating() const
{
	return PowerCapacity >= PowerConsumption;
}

void ACentralHubBuilding::ConnectToBuilding(ABuilding* Building)
{
	if (
		Building &&
		Building->GetBuildingType() == EBuildingType::EBT_HubLinkedFacility &&
		!ChildBuildings.Contains(Building) &&
		!Building->IsConnectedToParentBuilding()
		)
	{
		ChildBuildings.AddUnique(Building);
		Building->OnConnectToBuilding(this);
	}
}

void ACentralHubBuilding::DisconnectToBuilding(ABuilding* Building)
{
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_HubLinkedFacility && ChildBuildings.Contains(Building) && !Building->IsPreview())
	{
		ChildBuildings.Remove(Building);
		Building->OnDisconnectToBuilding();
	}
}

void ACentralHubBuilding::TryConnectToNearByFacility()
{
	Super::TryConnectToNearByFacility();
	TArray<FHitResult> HitResults;
	TraceBuildingsInBoundary(HitResults);
	for (FHitResult& HitResult : HitResults)
	{
		ABuilding* Building = Cast<ABuilding>(HitResult.GetActor());
		if (Building)
		{
			ConnectToBuilding(Building);
		}
	}
}

void ACentralHubBuilding::OnBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlapGridBoundary(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	ABuilding* Building = Cast<ABuilding>(OtherActor);
	ConnectToBuilding(Building);
}

void ACentralHubBuilding::OnEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlapGridBoundary(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	ABuilding* Building = Cast<ABuilding>(OtherActor);
	DisconnectToBuilding(Building);
}