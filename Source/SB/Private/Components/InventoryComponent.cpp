#include "Components/InventoryComponent.h"
#include "Items/PackagedItem.h"
#include "HUDs/InventoryWidget.h"
#include "HUDs/InventorySlotWidget.h"
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
	
	SetInventorySize(InventorySize);
}

void UInventoryComponent::SetInventoryItemInfo(const FItemData& Item, int i)
{
	if (Inventory.IsValidIndex(i))
	{
		Inventory[i] = Item;
		UpdateItemWidget(i);
	}
}

void UInventoryComponent::SetInventorySize(const int32& Size)
{
	InventorySize = Size;
	Inventory.SetNum(InventorySize);
}

void UInventoryComponent::UpdateItemWidget(int i)
{
	if (Inventory.IsValidIndex(i) && InventoryWidget)
	{
		Inventory[i].LinkedSlots.Remove(nullptr);
		TArray<UInventorySlotWidget*> InvalidSlots;
		for (UInventorySlotWidget* SlotWidget : Inventory[i].LinkedSlots)
		{
			if (SlotWidget->GetItemDataPtr() == &Inventory[i])
			{
				SlotWidget->Update();
			
			}
			else
			{
				InvalidSlots.Add(SlotWidget);
			}
		}
		for (UInventorySlotWidget* InvalidSlot : InvalidSlots)
		{
			Inventory[i].LinkedSlots.Remove(InvalidSlot);
		}
	}
}

bool UInventoryComponent::AddItem(const FItemData& Item)
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
			else if (Inventory[i].ItemClass == ItemClass && Inventory[i].bStackable)
			{
				SlotIndex = i;
				break;
			}
		}

		if (SlotIndex >= 0)
		{
			FItemData& InventoryItem = Inventory[SlotIndex];
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
		if (AddItem(Item->MakeItemData()))
		{
			Item->Destroy();
			return true;
		}
	}
	
	return false;
}

bool UInventoryComponent::AddItem(APackagedItem* PackagedItem)
{
	if (IsValid(PackagedItem))
	{
		if (AddItem(PackagedItem->GetItemData()))
		{
			PackagedItem->Destroy();
			return true;
		}
	}
	return false;
}

FItemData UInventoryComponent::RemoveItem(int Index, int Quantity)
{
	FItemData Item;
	if (Inventory.IsValidIndex(Index))
	{
		Item = Inventory[Index];
		Item.Quantity = Quantity < Item.Quantity ? Quantity : Item.Quantity;
		if (Item.Quantity > 0)
		{
			Inventory[Index].Quantity -= Quantity;
			UpdateItemWidget(Index);
		}
	}
	return Item;
}

TArray<AItem*> UInventoryComponent::DropItem(int Index, int Amount)
{
	FItemData Item = RemoveItem(Index, Amount);
	UWorld* World = GetWorld();
	TArray<AItem*> DroppedItem;
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
			DroppedItem.Add(World->SpawnActor<AItem>(Item.ItemClass, SpawnTransform));
		}
	}
	return DroppedItem;
}

void UInventoryComponent::LogItems()
{
	for (const FItemData& Item : Inventory)
	{
		if (Item.Quantity > 0)
		{
			SCREEN_LOG_SINGLE_FRAME(Item.Name.ToString() + ", " + FString::FromInt(Item.Quantity));
		}
	}
}
