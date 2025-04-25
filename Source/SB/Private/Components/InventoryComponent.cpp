#include "Components/InventoryComponent.h"
#include "Items/Item.h"
#include "HUDs/SBPlayerInventoryWidget.h"
#include "HUDs/ItemSlotWidget.h"
#include "SB/DebugMacro.h"

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

void UInventoryComponent::SetInventoryItemInfo(const FInventoryItemInfo& Item, int i)
{
	if (Inventory.IsValidIndex(i))
	{
		Inventory[i] = Item;
		UpdateItemWidget(i);
	}
}

void UInventoryComponent::UpdateItemWidget(int i)
{
	if (Inventory.IsValidIndex(i) && InventoryWidget)
	{
		InventoryWidget->UpdateItemSlotWidget(&Inventory[i], i);
	}
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
				SetInventoryItemInfo(Item, SlotIndex);
				return true;
			}
			else if (InventoryItem.bStackable)
			{
				InventoryItem.Quantity += Item.Quantity;
				UpdateItemWidget(SlotIndex);
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

FInventoryItemInfo UInventoryComponent::RemoveItem(int Index, int Quantity)
{
	FInventoryItemInfo Item;
	if (Inventory.IsValidIndex(Index))
	{
		Item = Inventory[Index];
		Item.Quantity = Quantity < Item.Quantity ? Quantity : Item.Quantity;
		if (Item.Quantity > 0)
		{
			SCREEN_LOG_NONE_KEY(TEXT("RemoveItem"));
			Inventory[Index].Quantity -= Quantity;
			UpdateItemWidget(Index);
		}
	}
	return Item;
}

void UInventoryComponent::DropItem(int Index, int Amount)
{
	FInventoryItemInfo Item = RemoveItem(Index, Amount);
	UWorld* World = GetWorld();
	if (World && Item.ItemClass)
	{
		AActor* Owner = GetOwner();
		FVector SpawnLocation;
		if (Owner)
		{
			SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 500.0f;
		}
		for (int i = 0; i < Item.Quantity; ++i)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnLocation);
			World->SpawnActor<AItem>(Item.ItemClass, SpawnTransform);
			SCREEN_LOG_NONE_KEY(TEXT("DropItem"));
		}
	}
}