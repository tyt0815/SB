#include "HUDs/InventoryWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "HUDs/InventorySlotWidget.h"
#include "SB/DebugMacro.h"

void UInventoryWidget::Update(UInventoryComponent* InventoryComponent)
{
	UWorld* World = GetWorld();
	if (World && InventoryComponent && ItemSlotClass)
	{
		InventoryComponent->SetInventoryWidget(this);
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			int32 InventorySize = InventoryComponent->GetInventorySize();
			for (int i = ItemSlots.Num(); i < InventorySize; ++i)
			{
				ItemSlots.Add(CreateWidget<UInventorySlotWidget>(Controller, ItemSlotClass));
				if (ItemSlots[i])
				{
					WrapBox->AddChild(ItemSlots[i]);
					ItemSlots[i]->SetIndex(i);
					UpdateItemSlotWidget(InventoryComponent->GetItemDataPtr(i), i);
				}
			}
		}
	}
}

void UInventoryWidget::UpdateItemSlotWidget(FItemData* Item, int i)
{
	if (ItemSlots.IsValidIndex(i) && ItemSlots[i])
	{
		ItemSlots[i]->SetItemData(Item);
	}
}