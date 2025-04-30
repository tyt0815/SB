// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProductionFacilityInfoWidget.generated.h"

class UInventoryWidget;
class UInventoryComponent;

UCLASS()
class SB_API UProductionFacilityInfoWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void Update(
		UInventoryComponent* PlayerInventoryComponent,
		UInventoryComponent* InputInventoryComponent,
		UInventoryComponent* OutputInventoryComponent
	);
private:		
	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* PlayerInventoryWidget;

	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* FacilityInputWidget;

	UPROPERTY(meta = (BindWidget))
	UInventoryWidget* FacilityOutputWidget;
};
