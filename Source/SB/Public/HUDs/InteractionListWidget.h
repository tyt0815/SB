// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionListWidget.generated.h"

class UInteractionDescriptionWidget;
class UVerticalBox;
class UInteractionComponent;

UCLASS()
class SB_API UInteractionListWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Enable(UInteractionComponent* InInteractionComponent);
	void Disable();
	void FocusAt(int32 i);

protected:
	UPROPERTY(EditAnywhere, Category = InteractionList)
	TSubclassOf<UInteractionDescriptionWidget> InteractionDescriptionWidgetClass;

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UInteractionComponent* InteractionComponent;
	TArray<UInteractionDescriptionWidget*> Descriptions;
	int32 SelectedOptionIndex = 0;
};
