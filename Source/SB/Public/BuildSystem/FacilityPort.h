#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/ConveyorBelt.h"
#include "FacilityPort.generated.h"

class UInventoryComponent;

UCLASS()
class SB_API AFacilityPort : public AConveyorBelt
{
	GENERATED_BODY()

public:
	virtual void Tick(float Delta) override;

public:
	void UpdateToken();

protected:
	UInventoryComponent* ConnectedInventory = nullptr;
	int32 Token = 0;

public:
	FORCEINLINE void SetConnectedInventory(UInventoryComponent* Inventory) 
	{
		ConnectedInventory = Inventory;
	}
};
