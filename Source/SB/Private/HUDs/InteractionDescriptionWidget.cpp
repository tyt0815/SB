#include "HUDs/InteractionDescriptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "SB/DebugMacro.h"

void UInteractionDescriptionWidget::Focus()
{
	SCREEN_LOG_NONE_KEY(TEXT("Focus"));
	Description->SetColorAndOpacity(FColor(255, 255, 255, 255));
	Background->SetVisibility(ESlateVisibility::Visible);
}

void UInteractionDescriptionWidget::Unfocus()
{
	Description->SetColorAndOpacity(FColor(255, 255, 255, 100));
	Background->SetVisibility(ESlateVisibility::Hidden);
}

void UInteractionDescriptionWidget::SetDescription(FText Text)
{
	Description->SetText(Text);
}
