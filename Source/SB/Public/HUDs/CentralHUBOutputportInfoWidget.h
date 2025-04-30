// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemData.h"
#include "CentralHUBOutputportInfoWidget.generated.h"

class UInventoryWidget;
class UInventorySlotWidget;
class UInventoryComponent;

UCLASS()
class SB_API UCentralHUBOutputportInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Update(UInventoryComponent* HUBStorageComponent, FItemData* OutputData);

private:
	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* HUBStorageWidget;

	UPROPERTY(meta = (BindWidget))
	UInventorySlotWidget* OutputDataWidget;
};
