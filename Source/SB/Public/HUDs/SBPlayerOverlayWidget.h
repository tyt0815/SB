#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBPlayerOverlayWidget.generated.h"

class UInventoryComponent;
class UInteractionComponent;

UCLASS()
class SB_API USBPlayerOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerInventoryVisibility(ESlateVisibility SlateVisibility);
	void SetPlayerInventoryEnabled(bool bEnable);
	void OpenInventoryWidget(UInventoryComponent* InventoryComponent);
	void CloseInventoryWidget();
	void UpdateInventory(UInventoryComponent* InventoryComponent);
	void ShowInteractionList(UInteractionComponent* InteractionComponent);
	void HideInteractionList();
	void FocusInteractionDescriptionAt(int32 i);
	
private:
	UPROPERTY(meta = (BindWidget))
	class USBPlayerInventoryWidget* PlayerInventory;

	UPROPERTY(meta = (BindWidget))
	class UInteractionListWidget* InteractionList;

	bool bInventoryOpened = false;

public:
	FORCEINLINE bool IsInventoryOpened() const
	{
		return bInventoryOpened;
	}
	FORCEINLINE USBPlayerInventoryWidget* GetPlayerInventoryWidget() const
	{
		return PlayerInventory;
	}
};
