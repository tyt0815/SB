#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemData.h"
#include "InventoryComponent.generated.h"

class AItem;
class APackagedItem;
class UInventoryWidget;


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
	void SetInventoryItemInfo(const FItemData& Item, int i);
	void SetInventorySize(const int32& Size);
	void UpdateItemWidget(int i);
	bool AddItem(const FItemData& Item);
	bool AddItem(AItem* Item);
	bool AddItem(APackagedItem* PackagedItem);
	FItemData RemoveItem(int Index, int Quantity);
	TArray<AItem*> DropItem(int Index, int Quantity);
	void LogItems();

protected:

private:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int InventorySize = 50;

	UInventoryWidget* InventoryWidget;
	TArray<FItemData> Inventory;

public:
	FORCEINLINE const TArray<FItemData>& GetInventory() const
	{
		return Inventory;
	}
	FORCEINLINE const int GetInventorySize() const
	{
		return InventorySize;
	}
	FORCEINLINE const FItemData& GetItemData(int i) const
	{
		return Inventory[i];
	}
	FORCEINLINE void SetInventoryWidget(UInventoryWidget* Widget)
	{
		InventoryWidget = Widget;
	}
	FORCEINLINE bool IsValidIndex(int32 Index) const
	{
		return Inventory.IsValidIndex(Index);
	}
};
