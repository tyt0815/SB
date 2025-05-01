#include "HUDs/InventorySlotWidget.h"
#include "HUDs/InventoryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/InventoryComponent.h"
#include "Items/ItemData.h"
#include "BuildSystem/OutputPort.h"
#include "SB/DebugMacro.h"

void UInventorySlotWidget::Update()
{
	if (ItemData)
	{
		SetQuantity(ItemData->Quantity);
		SetThumnail(ItemData->Thumbnail);
	}
	else
	{
		SetQuantity(0);
	}
}

void UInventorySlotWidget::SetQuantity(int Value)
{
	if (Value == 0)
	{
		Thumbnail->SetVisibility(ESlateVisibility::Hidden);
		Quantity->SetText(FText());
	}
	else
	{
		Thumbnail->SetVisibility(ESlateVisibility::Visible);
		Quantity->SetText(FText::AsNumber(Value));
	}
}

void UInventorySlotWidget::SetThumnail(UTexture2D* Texture2D)
{
	Thumbnail->SetBrushFromTexture(Texture2D);
}

void UInventorySlotWidget::SetItemData(FItemData* InItemData)
{
	if (ItemData)
	{
		ItemData->LinkedSlots.Remove(this);
	}

	ItemData = InItemData;
	if (ItemData)
	{
		ItemData->LinkedSlots.AddUnique(this);
		ItemData->LinkedComponent->UpdateItemWidget(ItemData);
	}
	Update();
}

void UInventorySlotWidget::CopyItemData(FItemData NewData)
{
	if (ItemData)
	{
		*ItemData = NewData;
		ItemData->LinkedSlots.AddUnique(this);
		ItemData->LinkedComponent->UpdateItemWidget(ItemData);
	}
}

void UInventorySlotWidget::SetLinkedOutputPort(AOutputPort* OutputPort)
{
	LinkedOutputPort = OutputPort;
	if (LinkedOutputPort)
	{
		SetItemData(LinkedOutputPort->GetLinkedItemData());
	}
}

void UInventorySlotWidget::SwapItemData(UInventorySlotWidget* OtherSlot)
{
	if (ItemData && OtherSlot && OtherSlot->GetItemDataPtr())
	{
		FItemData A = *ItemData;
		FItemData B = *OtherSlot->GetItemDataPtr();
		CopyItemData(B);
		OtherSlot->CopyItemData(A);
	}
}

void UInventorySlotWidget::LinkItemClass(UInventorySlotWidget* OtherSlot)
{
	if (OtherSlot && LinkedOutputPort)
	{
		FItemData* OtherItemData = OtherSlot->GetItemDataPtr();
		if (OtherItemData)
		{
			LinkedOutputPort->SetLinkedItemClass(OtherItemData->ItemClass);
			SetItemData(LinkedOutputPort->GetLinkedItemData());
		}
	}
}