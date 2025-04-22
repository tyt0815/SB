#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridBuilding.h"
#include "CentralHubBuilding.generated.h"

/**
 * 
 */
UCLASS()
class SB_API ACentralHubBuilding : public AGridBuilding
{
	GENERATED_BODY()
public:

protected:
	virtual void OnPlayerBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnPlayerEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
