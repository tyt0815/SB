#include "HUDs/InventorySlotWidget.h"
#include "HUDs/InventoryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "SB/DebugMacro.h"

void UInventorySlotWidget::NativeConstruct()
{
	if (Button)
	{
		Button->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnClicked);
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

void UInventorySlotWidget::OnClicked()
{
	if (InventoryWidget)
	{
		InventoryWidget->OnSlotClicked(Index);
	}
}
