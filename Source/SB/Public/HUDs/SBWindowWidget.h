// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBWindowWidget.generated.h"

class UInventoryWidget;
class UButton;
class UWidgetSwitcher;
class UInventoryComponent;

UCLASS()
class SB_API USBWindowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	void OpenPlayerInventory(UInventoryComponent* InventoryComponent);
	void Open(UUserWidget* Widget);

	UFUNCTION()
	void Close();
	
private:
	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* PlayerInventoryWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
};
