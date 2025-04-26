// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionDescriptionWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class SB_API UInteractionDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Focus();
	void Unfocus();
	void SetDescription(FText Text);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Background;
};
