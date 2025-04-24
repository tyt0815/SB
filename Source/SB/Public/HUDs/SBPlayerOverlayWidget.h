#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBPlayerOverlayWidget.generated.h"

class UInventoryComponent;

UCLASS()
class SB_API USBPlayerOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerInventoryVisibility(ESlateVisibility SlateVisibility);
	void OpenInventoryWidget(UInventoryComponent* InventoryComponent);
	void CloseInventoryWidget();
	void UpdateInventory(UInventoryComponent* InventoryComponent);
	
private:
	UPROPERTY(meta = (BindWidget))
	class USBPlayerInventoryWidget* PlayerInventory;

	bool bInventoryOpened = false;

public:
	FORCEINLINE bool IsInventoryOpened() const
	{
		return bInventoryOpened;
	}
};
