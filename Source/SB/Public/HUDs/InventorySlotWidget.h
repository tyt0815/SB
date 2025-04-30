// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UInventoryWidget;

UCLASS()
class SB_API UInventorySlotWidget : public UUserWidget
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
	UImage* Thumbnail;

	UInventoryWidget* InventoryWidget;

	int Index = -1;
public:
	FORCEINLINE void SetIndex(int i)
	{
		Index = i;
	}
	FORCEINLINE void SetInventoryWidget(UInventoryWidget* InInventoryWidget)
	{
		InventoryWidget = InInventoryWidget;
	}
};
