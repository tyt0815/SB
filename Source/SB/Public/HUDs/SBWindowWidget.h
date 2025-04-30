// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemData.h"
#include "SBWindowWidget.generated.h"

class UInventoryWidget;
class UButton;
class UWidgetSwitcher;
class UInventoryComponent;
class UProductionFacilityInfoWidget;
class UCentralHUBOutputportInfoWidget;
class UCentralHUBInfoWidget;

UCLASS()
class SB_API USBWindowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void OpenPlayerInventory(UInventoryComponent* InventoryComponent);
	void OpenProductionFacilityInfoWidget(
		UInventoryComponent* PlayerInventory,
		UInventoryComponent* InputInventory,
		UInventoryComponent* OutputInventory
	);
	void OpenCentralHUBOutputportInfoWidget(
		UInventoryComponent* HUBStorageComponent,
		FItemData* OutputData
	);
	void OpenCentralHUBInfoWidget(
		UInventoryComponent* HUBStorageComponent,
		UInventoryComponent* PlayerInventoryComponent
	);
	void Open(UUserWidget* Widget);

	UFUNCTION()
	void Close();
	
private:
	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* PlayerInventoryWidget;

	UPROPERTY(meta = (BindWidget))
	UProductionFacilityInfoWidget* ProductionFacilityInfoWidget;

	UPROPERTY(meta = (BindWidget))
	UCentralHUBOutputportInfoWidget* CentralHUBOutputPortInfoWidget;

	UPROPERTY(meta = (BindWidget))
	UCentralHUBInfoWidget* CentralHUBInfoWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
};
