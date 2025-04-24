#include "Components/InventoryComponent.h"
#include "Items/Item.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Inventory.SetNum(InventorySize);
}

FInventoryItemInfo UInventoryComponent::ToInventoryItemInfo(AItem* Item)
{
	FInventoryItemInfo ItemInfo;
	ItemInfo.ItemClass = Item->GetClass();
	ItemInfo.Name = Item->GetItemName();
	ItemInfo.bStackable = Item->IsStackable();
	ItemInfo.Thumnail = Item->GetThumnail();
	ItemInfo.Quantity = 1;
	return ItemInfo;
}

bool UInventoryComponent::AddItem(const FInventoryItemInfo& Item)
{
	if (Item.Quantity > 0 && Item.ItemClass)
	{
		TSubclassOf<AItem> ItemClass = Item.ItemClass;
		int SlotIndex = -1;
		for (int i = 0; i < InventorySize; ++i)
		{
			if (Inventory[i].Quantity == 0)
			{
				if (SlotIndex < 0)
				{
					SlotIndex = i;
				}
			}
			else if (Inventory[i].ItemClass == ItemClass)
			{
				SlotIndex = i;
				break;
			}
		}

		if (SlotIndex >= 0)
		{
			FInventoryItemInfo& InventoryItem = Inventory[SlotIndex];
			if (InventoryItem.Quantity < 1)
			{
				InventoryItem = Item;
				return true;
			}
			else if (InventoryItem.bStackable)
			{
				InventoryItem.Quantity += 1;
				return true;
			}
		}
	}
	return false;
}

bool UInventoryComponent::AddItem(AItem* Item)
{
	if (Item)
	{
		return AddItem(ToInventoryItemInfo(Item));
	}
	
	return false;
}

AItem* UInventoryComponent::TakeItem(int i)
{
	AItem* Item = nullptr;
	UWorld* World = GetWorld();
	if (World && Inventory.IsValidIndex(i) && Inventory[i].Quantity > 0 && Inventory[i].ItemClass)
	{
		World->SpawnActor<AItem>(Inventory[i].ItemClass);
	}
	return Item;
}


