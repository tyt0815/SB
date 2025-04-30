#include "HUDs/InventorySlotWidget.h"
#include "HUDs/InventoryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Items/ItemData.h"
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
	}
	Update();
}

void UInventorySlotWidget::CopyItemData(FItemData NewData)
{
	if (ItemData)
	{
		SCREEN_LOG_NONE_KEY(TEXT("siba2"));
		*ItemData = NewData;
		ItemData->LinkedSlots.AddUnique(this);
		Update();
	}
}

void UInventorySlotWidget::SwapItemData(UInventorySlotWidget* OtherSlot)
{
	if (ItemData && OtherSlot && OtherSlot->GetItemDataPtr())
	{
		SCREEN_LOG_NONE_KEY(TEXT("siba"));
		FItemData A = *ItemData;
		A.LinkedSlots.Remove(this);
		FItemData B = *OtherSlot->GetItemDataPtr();
		B.LinkedSlots.Remove(OtherSlot);
		CopyItemData(B);
		OtherSlot->CopyItemData(A);
	}
}