#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBPlayerInventoryWidget.generated.h"

class UWrapBox;
class UItemSlotWidget;
class UInventoryComponent;
struct FInventoryItemInfo;

UCLASS()
class SB_API USBPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateInventory(UInventoryComponent* InventoryComponent);
	void UpdateItemSlotWidget(const FInventoryItemInfo* const Item, int i);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SBPlayerInventoryWidget")
	TSubclassOf<UItemSlotWidget> ItemSlotClass;
	
private:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox;

	TArray<UItemSlotWidget*> ItemSlots;
};
