// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class SB_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quantity;

	UPROPERTY(meta = (BindWidget))
	UImage* Thumnail;
};
