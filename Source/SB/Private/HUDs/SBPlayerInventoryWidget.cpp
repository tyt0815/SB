#include "HUDs/SBPlayerInventoryWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"
#include "HUDs/ItemSlotWidget.h"
#include "SB/DebugMacro.h"

void USBPlayerInventoryWidget::UpdateInventory(UInventoryComponent* InventoryComponent)
{
	UWorld* World = GetWorld();
	if (World && InventoryComponent && ItemSlotClass)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			const TArray<FInventoryItemInfo>& Inventory = InventoryComponent->GetInventory();
			for (int i = ItemSlots.Num(); i < Inventory.Num(); ++i)
			{
				ItemSlots.Add(CreateWidget<UItemSlotWidget>(Controller, ItemSlotClass));
				if (ItemSlots[i])
				{
					WrapBox->AddChild(ItemSlots[i]);
					ItemSlots[i]->SetIndex(i);
					ItemSlots[i]->SetInventory(InventoryComponent);
					UpdateItemSlotWidget(&InventoryComponent->GetItemInfo(i), i);
				}
			}
		}
	}
}

void USBPlayerInventoryWidget::UpdateItemSlotWidget(const FInventoryItemInfo* const Item, int i)
{
	if (ItemSlots.IsValidIndex(i))
	{
		UItemSlotWidget* Widget = ItemSlots[i];
		Widget->SetQuantity(Item->Quantity);
		Widget->SetThumnail(Item->Thumnail);
	}
}