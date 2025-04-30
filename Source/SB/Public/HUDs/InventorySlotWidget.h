// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UInventoryWidget;
struct FItemData;

UCLASS()
class SB_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Update();
	void SetQuantity(int Value);
	void SetThumnail(UTexture2D* Texture2D);
	void SetItemData(FItemData* InItemData);
	
private:

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quantity;

	UPROPERTY(meta = (BindWidget))
	UImage* Thumbnail;

	FItemData* ItemData = nullptr;

	int Index = -1;
public:
	FORCEINLINE void SetIndex(int i)
	{
		Index = i;
	}
	FORCEINLINE FItemData* GetItemDataPtr() const
	{
		return ItemData;
	}
};
