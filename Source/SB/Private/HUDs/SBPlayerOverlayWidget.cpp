#include "HUDs/SBPlayerOverlayWidget.h"
#include "HUDs/SBWindowWidget.h"
#include "HUDs/InteractionListWidget.h"
#include "SB/DebugMacro.h"

void USBPlayerOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	WindowWidget->Close();
}

void USBPlayerOverlayWidget::OpenInventoryWidget(UInventoryComponent* InventoryComponent)
{
	WindowWidget->OpenPlayerInventory(InventoryComponent);
}

void USBPlayerOverlayWidget::OpenProductionFacilityInfoWidget(
	UInventoryComponent* PlayerInventoryComponent,
	UInventoryComponent* InputInventoryComponent,
	UInventoryComponent* OutputInventoryComponent
)
{
	WindowWidget->OpenProductionFacilityInfoWidget(PlayerInventoryComponent, InputInventoryComponent, OutputInventoryComponent);
}

void USBPlayerOverlayWidget::ShowInteractionList(UInteractionComponent* InteractionComponent)
{
	InteractionList->Enable(InteractionComponent);
}

void USBPlayerOverlayWidget::HideInteractionList()
{
	InteractionList->Disable();
}

void USBPlayerOverlayWidget::FocusInteractionDescriptionAt(int32 i)
{
	InteractionList->FocusAt(i);
}
