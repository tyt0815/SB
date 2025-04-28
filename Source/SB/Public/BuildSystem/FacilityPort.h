#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FacilityPort.generated.h"

class UInventoryComponent;

UCLASS()
class SB_API AFacilityPort : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AFacilityPort();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	void UpdateToken();

protected:
	UPROPERTY(VisibleAnywhere, Category = "OutputPort")
	UStaticMeshComponent* StaticMesh;

	UInventoryComponent* ConnectedInventory = nullptr;
	int32 Token = 0;

public:
	FORCEINLINE void SetConnectedInventory(UInventoryComponent* Inventory) 
	{
		ConnectedInventory = Inventory;
	}
};
