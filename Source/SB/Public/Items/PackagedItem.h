// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "PackagedItem.generated.h"

/**
 * 
 */
UCLASS()
class SB_API APackagedItem : public AItem
{
	GENERATED_BODY()

public:
	void SetItemData(FItemData Item);

protected:
	virtual void BeginPlay() override;
	virtual void InitMeshsVisibilityAndPhysics() override {};

	UFUNCTION(BlueprintImplementableEvent, Category = "Item | PackagedItem")
	void SetLabelTexture(UTexture2D* Thumnail);
	
protected:
	FItemData ItemData;
};
