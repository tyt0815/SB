#include "HUDs/CentralHUBOutputportInfoWidget.h"
#include "HUDs/InventoryWidget.h"
#include "HUDs/InventorySlotWidget.h"

void UCentralHUBOutputportInfoWidget::Update(UInventoryComponent* HUBStorageComponent, AOutputPort* OutputPort)
{
	HUBStorageWidget->Update(HUBStorageComponent);
	OutputDataWidget->SetLinkedOutputPort(OutputPort);
}
