#include "HUDs/SBWindowWidget.h"
#include "HUDs/InventoryWidget.h"
#include "HUDs/ProductionFacilityInfoWidget.h"
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
	PlayerInventoryWidget->Update(InventoryComponent);
}

void USBWindowWidget::OpenProductionFacilityInfoWidget(
	UInventoryComponent* PlayerInventory,
	UInventoryComponent* InputInventory,
	UInventoryComponent* OutputInventory
)
{
	Open(ProductionFacilityInfoWidget);
	ProductionFacilityInfoWidget->Update(PlayerInventory, InputInventory, OutputInventory);
}

void USBWindowWidget::Open(UUserWidget* Widget)
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
	WidgetSwitcher->SetActiveWidget(Widget);
	UWorld* World = GetWorld();
	if (World)
	{
		ASBPlayerController* Controller = Cast<ASBPlayerController>(World->GetFirstPlayerController());
		if (Controller)
		{
			Controller->SwitchToUIMode(true);
		}
	}
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
