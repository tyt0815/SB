// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/ItemData.h"
#include "InventorySlotWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UInventoryWidget;
class AOutputPort;

UCLASS()
class SB_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Update();
	void SetQuantity(int Value);
	void SetThumnail(UTexture2D* Texture2D);
	void SetItemData(FItemData* InItemData);
	void CopyItemData(FItemData NewData);
	void SetLinkedOutputPort(AOutputPort* OutputPort);

	UFUNCTION(BlueprintCallable, Category = "InventorySlotWidget")
	void SwapItemData(UInventorySlotWidget* OtherSlot);

	UFUNCTION(BlueprintCallable, Category = "InventorySlotWidget")
	void LinkItemClass(UInventorySlotWidget* OtherSlot);
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "InventorySlotWidget", meta = (BindWidget))
	UButton* Button;

	UPROPERTY(BlueprintReadOnly, Category = "InventorySlotWidget", meta = (BindWidget))
	UTextBlock* Quantity;

	UPROPERTY(BlueprintReadOnly, Category = "InventorySlotWidget", meta = (BindWidget))
	UImage* Thumbnail;

	FItemData* ItemData = nullptr;
	AOutputPort* LinkedOutputPort;

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
