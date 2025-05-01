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
		auto a = Inventory[i].LinkedSlots;
		Inventory[i] = Item;
		Inventory[i].LinkedSlots = a;
		UpdateItemWidgetAt(i);
	}
}

void UInventoryComponent::SetInventorySize(const int32& Size)
{
	InventorySize = FMath::Max(1, Size);
	Inventory.SetNum(InventorySize);
	for (auto& Item : Inventory)
	{
		Item.LinkedComponent = this;
	}
}

void UInventoryComponent::UpdateItemWidgetAt(int i)
{
	if (Inventory.IsValidIndex(i))
	{
		UpdateItemWidget(&Inventory[i]);
	}
}

void UInventoryComponent::UpdateItemWidget(FItemData* Target)
{
	if (Target)
	{
		Target->LinkedSlots.Remove(nullptr);
		TArray<UInventorySlotWidget*> InvalidSlots;
		for (UInventorySlotWidget* SlotWidget : Target->LinkedSlots)
		{
			if (IsValid(SlotWidget) && SlotWidget->GetItemDataPtr() == Target)
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
			Target->LinkedSlots.Remove(InvalidSlot);
		}
	}
	
}

int32 UInventoryComponent::FindItemIndex(TSubclassOf<AItem> ItemClass)
{
	for (int32 i = 0; i < InventorySize; ++i)
	{
		if (Inventory[i].Quantity > 0 && Inventory[i].ItemClass == ItemClass)
		{
			return i;
		}
	}
	return -1;
}

int32 UInventoryComponent::FindEmptySlotIndex()
{
	for (int32 i = 0; i < InventorySize; ++i)
	{
		if (Inventory[i].Quantity == 0)
		{
			return i;
		}
	}
	return -1;
}

bool UInventoryComponent::AddItem(const FItemData& Item)
{
	if (Item.Quantity > 0 && Item.ItemClass)
	{
		int32 Index = FindItemIndex(Item.ItemClass);
		if (Index < 0)
		{
			Index = FindEmptySlotIndex();
			if (Index < 0)
			{
				return false;
			}
			else
			{
				SetInventoryItemInfo(Item, Index);
			}
		}
		else
		{	
			Inventory[Index].Quantity += Item.Quantity;
			UpdateItemWidgetAt(Index);
		}
	}
	return true;
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
			UpdateItemWidgetAt(Index);
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
