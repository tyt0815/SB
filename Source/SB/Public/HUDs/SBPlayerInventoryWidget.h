#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBPlayerInventoryWidget.generated.h"

class UWrapBox;
class UItemSlotWidget;
class UInventoryComponent;

UCLASS()
class SB_API USBPlayerInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateInventory(UInventoryComponent* InventoryComponent);
	
private:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UItemSlotWidget> ItemSlotWidgetClass;

	TArray<UItemSlotWidget*> ItemSlots;
};
