#include "HUDs/SBPlayerInventoryWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"
#include "HUDs/ItemSlotWidget.h"

void USBPlayerInventoryWidget::UpdateInventory(UInventoryComponent* InventoryComponent)
{
	auto& Inventory = InventoryComponent->GetInventory();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && ItemSlotWidgetClass)
		{
			for (int i = ItemSlots.Num(); i < Inventory.Num(); ++i)
			{
				ItemSlots.Add(CreateWidget<UItemSlotWidget>(Controller, ItemSlotWidgetClass));
				WrapBox->AddChild(ItemSlots[i]);
			}
		}
	}
	
}