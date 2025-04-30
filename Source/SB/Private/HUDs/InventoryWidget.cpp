#include "HUDs/InventoryWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/WrapBox.h"
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
			const TArray<FItemData>& Inventory = InventoryComponent->GetInventory();
			for (int i = ItemSlots.Num(); i < Inventory.Num(); ++i)
			{
				ItemSlots.Add(CreateWidget<UInventorySlotWidget>(Controller, ItemSlotClass));
				if (ItemSlots[i])
				{
					WrapBox->AddChild(ItemSlots[i]);
					ItemSlots[i]->SetIndex(i);
					ItemSlots[i]->SetInventoryWidget(this);
					UpdateItemSlotWidget(&InventoryComponent->GetItemData(i), i);
				}
			}
		}
	}
}

void UInventoryWidget::UpdateItemSlotWidget(const FItemData* const Item, int i)
{
	if (ItemSlots.IsValidIndex(i))
	{
		UInventorySlotWidget* Widget = ItemSlots[i];
		Widget->SetQuantity(Item->Quantity);
		Widget->SetThumnail(Item->Thumbnail);
	}
}

void UInventoryWidget::OnSlotClicked(int32 Index)
{
	if (ItemSlots.IsValidIndex(Index))
	{
		SCREEN_LOG(5, FString::FromInt(Index));
	}
}
