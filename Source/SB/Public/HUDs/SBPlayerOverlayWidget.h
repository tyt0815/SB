#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBPlayerOverlayWidget.generated.h"

class UInventoryComponent;
class UInteractionComponent;
class USBWindowWidget;

UCLASS()
class SB_API USBPlayerOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void OpenInventoryWidget(UInventoryComponent* InventoryComponent);
	void OpenProductionFacilityInfoWidget(
		UInventoryComponent* PlayerInventoryComponent,
		UInventoryComponent* InputInventoryComponent,
		UInventoryComponent* OutputInventoryComponent
	);
	void ShowInteractionList(UInteractionComponent* InteractionComponent);
	void HideInteractionList();
	void FocusInteractionDescriptionAt(int32 i);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UInteractionListWidget* InteractionList;

	UPROPERTY(meta = (BindWidget))
	USBWindowWidget* WindowWidget;

public:
};
