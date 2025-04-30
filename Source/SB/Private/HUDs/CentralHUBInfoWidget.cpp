#include "HUDs/CentralHUBInfoWidget.h"
#include "HUDs/InventoryWidget.h"

void UCentralHUBInfoWidget::Update(UInventoryComponent* HUBStorageComponent, UInventoryComponent* PlayerInventoryComponent)
{
	HUBStorageWidget->Update(HUBStorageComponent);
	PlayerInventoryWidget->Update(PlayerInventoryComponent);
}
