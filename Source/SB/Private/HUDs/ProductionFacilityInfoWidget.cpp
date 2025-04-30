// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/ProductionFacilityInfoWidget.h"
#include "HUDs/InventoryWidget.h"

void UProductionFacilityInfoWidget::Update(
	UInventoryComponent* PlayerInventoryComponent,
	UInventoryComponent* InputInventoryComponent,
	UInventoryComponent* OutputInventoryComponent
)
{
	PlayerInventoryWidget->Update(PlayerInventoryComponent);
	FacilityInputWidget->Update(InputInventoryComponent);
	FacilityOutputWidget->Update(OutputInventoryComponent);
}
