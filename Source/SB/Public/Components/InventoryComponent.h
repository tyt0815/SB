#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItem;

struct FInventoryItemInfo
{
	TSubclassOf<AItem> ItemClass = nullptr;
	UTexture2D* Thumnail = nullptr;
	FName Name;
	int Quantity = 0;
	bool bStackable = false;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SB_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:	
	FInventoryItemInfo ToInventoryItemInfo(AItem* Item);
	bool AddItem(const FInventoryItemInfo& Item);
	bool AddItem(AItem* Item);
	AItem* TakeItem(int i);

private:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int InventorySize = 50;

	TArray<FInventoryItemInfo> Inventory;

public:
	FORCEINLINE const TArray<FInventoryItemInfo>& GetInventory() const
	{
		return Inventory;
	}
};
