#include "HUDs/ItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/InventoryComponent.h"
#include "SB/DebugMacro.h"

void UItemSlotWidget::NativeConstruct()
{
	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UItemSlotWidget::OnClicked);
	}
}

void UItemSlotWidget::SetQuantity(int Value)
{
	if (Value == 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
		Quantity->SetText(FText::AsNumber(Value));
	}
}

void UItemSlotWidget::SetThumnail(UTexture2D* Texture2D)
{
	Thumbnail->SetBrushFromTexture(Texture2D);
}

void UItemSlotWidget::OnClicked()
{
	if (Inventory)
	{
		Inventory->DropItem(Index, 1);
	}
}
