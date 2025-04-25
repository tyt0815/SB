#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AItem;
class USBPlayerInventoryWidget;

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
	void SetInventoryItemInfo(const FInventoryItemInfo& Item, int i);
	void UpdateItemWidget(int i);
	FInventoryItemInfo ToInventoryItemInfo(AItem* Item);
	bool AddItem(const FInventoryItemInfo& Item);
	bool AddItem(AItem* Item);
	FInventoryItemInfo RemoveItem(int Index, int Quantity);
	void DropItem(int Index, int Quantity);

protected:

private:
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int InventorySize = 50;

	USBPlayerInventoryWidget* InventoryWidget;
	TArray<FInventoryItemInfo> Inventory;

public:
	FORCEINLINE const TArray<FInventoryItemInfo>& GetInventory() const
	{
		return Inventory;
	}
	FORCEINLINE const int GetInventorySize() const
	{
		return InventorySize;
	}
	FORCEINLINE const FInventoryItemInfo& GetItemInfo(int i) const
	{
		return Inventory[i];
	}
	FORCEINLINE void SetInventoryWidget(USBPlayerInventoryWidget* Widget)
	{
		InventoryWidget = Widget;
	}
};
