#include "HUDs/CentralHUBOutputportInfoWidget.h"
#include "HUDs/InventoryWidget.h"
#include "HUDs/InventorySlotWidget.h"

void UCentralHUBOutputportInfoWidget::Update(UInventoryComponent* HUBStorageComponent, FItemData* OutputData)
{
	HUBStorageWidget->Update(HUBStorageComponent);
	OutputDataWidget->SetItemData(OutputData);
}
