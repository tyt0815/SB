// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/SBWindowWidget.h"
#include "HUDs/SBPlayerInventoryWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerController/SBPlayerController.h"
#include "SB/DebugMacro.h"

void USBWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &USBWindowWidget::Close);
}

void USBWindowWidget::OpenPlayerInventory(UInventoryComponent* InventoryComponent)
{
	Open(PlayerInventoryWidget);
	PlayerInventoryWidget->UpdateInventory(InventoryComponent);
}

void USBWindowWidget::Open(UUserWidget* Widget)
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
	WidgetSwitcher->SetActiveWidget(Widget);
}

void USBWindowWidget::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
	UWorld* World = GetWorld();
	if (World)
	{
		ASBPlayerController* Controller = Cast<ASBPlayerController>(World->GetFirstPlayerController());
		if (Controller)
		{
			Controller->SwitchToUIMode(false);
		}
	}
}
