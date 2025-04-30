#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridBuilding.h"
#include "CentralHubBuilding.generated.h"

class UInventoryComponent;

UCLASS()
class SB_API ACentralHubBuilding : public AGridBuilding
{
	GENERATED_BODY()
public:
	ACentralHubBuilding();
	virtual void Tick(float Delta) override;

public:
	virtual void PropagatePowerState() override;
	virtual bool IsOperating() const override;
	void ConnectToBuilding(ABuilding* Building);
	void DisconnectToBuilding(ABuilding* Building);
	virtual void TryConnectToNearByFacility() override;

protected:
	virtual void AddInteractions() override;
	virtual void OnBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInventoryComponent* StorageComponent;

	UPROPERTY(EditAnywhere, Category = "Item | Building | GridBuilding | CentralHubBuilding")
	TSubclassOf<class AInputPort> InputPortClass;

	UPROPERTY(EditAnywhere, Category = "Item | Building | GridBuilding | CentralHubBuilding")
	TSubclassOf<class AOutputPort> OutputPortClass;

	UPROPERTY(EditAnywhere, Category = "Item | Building | GridBuilding | CentralHubBuilding")
	TArray<FIntVector> InputPortGridCoord;

	UPROPERTY(EditAnywhere, Category = "Item | Building | GridBuilding | CentralHubBuilding")
	TArray<FIntVector> OutputPortGridCoord;

	TArray<UChildActorComponent*> InputPortComponents;
	TArray<UChildActorComponent*> OutputPortComponents;
	int PowerCapacity = 500;
	int PrevPowerCapacity = 0;
	int PrevPowerConsumption = 0;

private:
	UFUNCTION()
	void ShowInfo(AActor* OtherActor);
};
