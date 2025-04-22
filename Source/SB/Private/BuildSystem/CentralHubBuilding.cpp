#include "BuildSystem/CentralHubBuilding.h"

void ACentralHubBuilding::OnPlayerBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnPlayerBeginOverlapGridBoundary(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	ABuilding* Building = Cast<ABuilding>(OtherActor);
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_HubLinkedFacility)
	{
		LinkedBuilding.AddUnique(Building);
		Building->ConnectToHub(this);
	}
}

void ACentralHubBuilding::OnPlayerEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnPlayerEndOverlapGridBoundary(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	ABuilding* Building = Cast<ABuilding>(OtherActor);
	if (Building && Building->GetBuildingType() == EBuildingType::EBT_HubLinkedFacility)
	{
		LinkedBuilding.Remove(Building);
		Building->ConnectToHub(this);
	}
}
