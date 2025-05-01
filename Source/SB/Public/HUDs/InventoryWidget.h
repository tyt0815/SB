#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UWrapBox;
class UInventorySlotWidget;
class UInventoryComponent;
struct FItemData;

UCLASS()
class SB_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Update(UInventoryComponent* InventoryComponent);

protected:
	void AdjustItemSlotsNum(UInventoryComponent* InventoryComponent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SBPlayerInventoryWidget")
	TSubclassOf<UInventorySlotWidget> ItemSlotClass;
	
private:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox;

	TArray<UInventorySlotWidget*> ItemSlots;
};
