// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CentralHUBInfoWidget.generated.h"

class UInventoryWidget;
class UInventoryComponent;

UCLASS()
class SB_API UCentralHUBInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Update(UInventoryComponent* HUBStorageComponent, UInventoryComponent* PlayerInventoryComponent);
	
private:
	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* HUBStorageWidget;

	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* PlayerInventoryWidget;
};
