#include "HUDs/InteractionListWidget.h"
#include "HUDs/InteractionDescriptionWidget.h"
#include "Components/InteractionComponent.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "SB/DebugMacro.h"

void UInteractionListWidget::Enable(UInteractionComponent* InInteractionComponent)
{
	UWorld* World = GetWorld();
	if (World&& InInteractionComponent && InInteractionComponent != InteractionComponent)
	{
		InteractionComponent = InInteractionComponent;
		int32 InteractionNum = InteractionComponent->GetInteractionNum();
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && InteractionDescriptionWidgetClass && InteractionNum > 0)
		{
			for (int i = Descriptions.Num(); i < InteractionNum; ++i)
			{
				Descriptions.Add(CreateWidget<UInteractionDescriptionWidget>(Controller, InteractionDescriptionWidgetClass));
				VerticalBox->AddChild(Descriptions[i]);
			}
			for (int i = InteractionNum; i < Descriptions.Num(); ++i)
			{
				Descriptions[i]->SetVisibility(ESlateVisibility::Hidden);
			}

			for (int i = 0; i < InteractionNum; ++i)
			{
				Descriptions[i]->SetVisibility(ESlateVisibility::Visible);
				Descriptions[i]->SetDescription(FText::FromName(InteractionComponent->GetInteractionDescriptionAt(i)));
				Descriptions[i]->Unfocus();
			}
			SelectedOptionIndex = 0;
			Descriptions[SelectedOptionIndex]->Focus();
		}
		
	}
}

void UInteractionListWidget::Disable()
{
	if (InteractionComponent)
	{
		InteractionComponent = nullptr;
		for (UInteractionDescriptionWidget* Widget : Descriptions)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInteractionListWidget::FocusAt(int32 i)
{
	if (Descriptions.IsValidIndex(i) && SelectedOptionIndex != i)
	{
		Descriptions[SelectedOptionIndex]->Unfocus();

		SelectedOptionIndex = i;
		Descriptions[SelectedOptionIndex]->Focus();
	}
}
