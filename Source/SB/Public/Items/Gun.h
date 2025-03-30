// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon.h"
#include "Gun.generated.h"

/**
 * 
 */
UCLASS()
class SB_API AGun : public AWeapon
{
	GENERATED_BODY()
	
public:
	AGun();

protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

private:
	
};
