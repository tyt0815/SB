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

	virtual void Tick(float Delta) override;

public:
	virtual void PropagatePowerState() override;
	virtual bool IsOperating() const override;
	void ConnectToBuilding(ABuilding* Building);
	void DisconnectToBuilding(ABuilding* Building);
	virtual void TryConnectToNearByFacility() override;

protected:
	virtual void OnBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	int PowerCapacity = 500;
	int PrevPowerCapacity = 0;
	int PrevPowerConsumption = 0;
};
