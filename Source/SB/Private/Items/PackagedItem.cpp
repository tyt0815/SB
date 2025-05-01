// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PackagedItem.h"
#include "SB/DebugMacro.h"

void APackagedItem::SetItemData(FItemData Item)
{
	ItemData = Item;

	SetLabelTexture(ItemData.Thumbnail);

	bSimulatePhysics = false;
}

void APackagedItem::BeginPlay()
{
	Super::BeginPlay();

	// TODO:
	ItemData = MakeItemData();

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}