#include "BuildSystem/PowerFacility.h"

APowerFacility::APowerFacility()	
{
	BuildingType = EBuildingType::EBT_HubLinkedFacility;
}

void APowerFacility::OnPlayerBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPlayerBeginOverlapGridBoundary(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	ABuilding* Building = Cast<ABuilding>(OtherActor);
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_PowerLinkedFacility)
	{
		LinkedBuilding.AddUnique(Building);
		Building->ConnectToPowerFacility(this);
	}
}

void APowerFacility::OnPlayerEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPlayerEndOverlapGridBoundary(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	ABuilding* Building = Cast<ABuilding>(OtherActor);
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_PowerLinkedFacility)
	{
		LinkedBuilding.Remove(Building);
		Building->DisconnectToPowerFacility(this);
	}
}
