// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UInventoryComponent;

UCLASS()
class SB_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	void SetQuantity(int Value);
	void SetThumnail(UTexture2D* Texture2D);
	
private:
	UFUNCTION()
	void OnClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quantity;

	UPROPERTY(meta = (BindWidget))
	UImage* Thumnail;

	UInventoryComponent* Inventory;

	int Index = -1;
public:
	FORCEINLINE void SetIndex(int i)
	{
		Index = i;
	}
	FORCEINLINE void SetInventory(UInventoryComponent* InInventory)
	{
		Inventory = InInventory;
	}
};
