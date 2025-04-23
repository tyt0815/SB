#include "BuildSystem/CentralHubBuilding.h"
#include "BuildSystem/PowerFacility.h"

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
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_HubLinkedFacility && !ChildBuildings.Contains(Building))
	{
		ChildBuildings.AddUnique(Building);
		Building->OnConnectToBuilding(this);
	}
}

void ACentralHubBuilding::DisconnectToBuilding(ABuilding* Building)
{
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_HubLinkedFacility && ChildBuildings.Contains(Building))
	{
		ChildBuildings.Remove(Building);
		Building->OnDisconnectToBuilding();
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
