#include "HUDs/SBPlayerOverlayWidget.h"
#include "HUDs/SBPlayerInventoryWidget.h"
#include "Components/InventoryComponent.h"
#include "SB/DebugMacro.h"

void USBPlayerOverlayWidget::SetPlayerInventoryVisibility(ESlateVisibility SlateVisibility)
{
	if (PlayerInventory)
	{
		PlayerInventory->SetVisibility(SlateVisibility);
	}
}

void USBPlayerOverlayWidget::OpenInventoryWidget(UInventoryComponent* InventoryComponent)
{
	bInventoryOpened = true;
	SetPlayerInventoryVisibility(ESlateVisibility::Visible);
	UpdateInventory(InventoryComponent);
}

void USBPlayerOverlayWidget::CloseInventoryWidget()
{
	bInventoryOpened = false;
	SetPlayerInventoryVisibility(ESlateVisibility::Hidden);
}

void USBPlayerOverlayWidget::UpdateInventory(UInventoryComponent* InventoryComponent)
{
	if (PlayerInventory && InventoryComponent)
	{
		PlayerInventory->UpdateInventory(InventoryComponent);
	}
}
