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
	virtual void Tick(float Delta) override;

protected:
	virtual void BeginPlay() override;

public:
	void SetBulletSpeed(float Speed);

protected:
	void InitializeActorsToIgnore();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitTrace)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// 다단히트를 발생시킬지를 정하는 값
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = HitTrace)
	bool bEnableMultiHit = true;

	// 다단 히트가 몇초에 한번 발생할지 정하는 값. bEnableMultiHit가 true일 경우에만 작동하는 옵션.
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = HitTrace)
	float DamageTickRate = 0.1;

	// 멀티 트레이스를 사용하는지 정하는 값. 사용할 시 한번의 히트에 다수의 액터가 포함될 수 있음
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = HitTrace)
	bool bEnableMultiTrace = false;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = HitTrace)
	bool bEnableHitTrace = true;

	TArray<FHitResult> HitResults;
	TArray<AActor*> HitActors;
private:
	void EvaluateHitActors();
	FVector CalculateHitTraceStartLocation();
	FVector CalculateHitTraceEndLocation();
	FVector CalculateHitTraceHalfSize();
	TArray<AActor*> ActorsToIgnore;
	FVector Location_LastFrame;

	UFUNCTION()
	void RemoveFromActorsToIgnore(AActor* Actor);

public:
	
};
