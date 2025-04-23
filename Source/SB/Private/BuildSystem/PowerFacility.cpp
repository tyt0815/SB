#include "BuildSystem/PowerFacility.h"
#include "BuildSystem/CentralHubBuilding.h"

APowerFacility::APowerFacility()	
{
	BuildingType = EBuildingType::EBT_HubLinkedFacility;
}

void APowerFacility::ConnectToBuilding(ABuilding* Building)
{
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_PowerLinkedFacility)
	{
		ChildBuildings.AddUnique(Building);
		Building->OnConnectToBuilding(this);
	}
}

void APowerFacility::DisconnectToBuilding(ABuilding* Building)
{
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_PowerLinkedFacility)
	{
		ChildBuildings.Remove(Building);
		Building->OnDisconnectToBuilding();
	}
}

void APowerFacility::PropagatePowerState()
{
	for (ABuilding* Building : ChildBuildings)
	{
		Building->PropagatePowerState();
	}
}

void APowerFacility::OnBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlapGridBoundary(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	ABuilding* Building = Cast<ABuilding>(OtherActor);
	ConnectToBuilding(Building);
}

void APowerFacility::OnEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlapGridBoundary(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	ABuilding* Building = Cast<ABuilding>(OtherActor);
	DisconnectToBuilding(Building);
}