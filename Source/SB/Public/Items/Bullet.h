// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Bullet.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class SB_API ABullet : public AItem
{
	GENERATED_BODY()
	
public:
	ABullet();
	virtual void BeginPlay() override;

public:
	void SetBulletSpeed(float Speed);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	UBoxComponent* BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	UProjectileMovementComponent* ProjectileMovementComponent;

public:
	
};
