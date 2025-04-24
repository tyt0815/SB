// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class SB_API AInventoryItem : public AItem
{
	GENERATED_BODY()
public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

protected:

public:
};
