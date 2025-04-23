#include "BuildSystem/PowerFacility.h"
#include "BuildSystem/CentralHubBuilding.h"
#include "SB/DebugMacro.h"

APowerFacility::APowerFacility()	
{
	BuildingType = EBuildingType::EBT_HubLinkedFacility;
}

void APowerFacility::ConnectToBuilding(ABuilding* Building)
{
	if (
		Building &&
		Building->GetBuildingType() == EBuildingType::EBT_PowerLinkedFacility &&
		!Building->IsConnectedToParentBuilding()
		)
	{
		ChildBuildings.AddUnique(Building);
		Building->OnConnectToBuilding(this);
	}
}

void APowerFacility::DisconnectToBuilding(ABuilding* Building)
{
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_PowerLinkedFacility && !Building->IsPreview())
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

void APowerFacility::TryConnectToNearByFacility()
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

void APowerFacility::OnBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SCREEN_LOG(1, TEXT("sibal"));
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