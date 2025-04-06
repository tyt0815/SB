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

	// �ٴ���Ʈ�� �߻���ų���� ���ϴ� ��
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = HitTrace)
	bool bEnableMultiHit = true;

	// �ٴ� ��Ʈ�� ���ʿ� �ѹ� �߻����� ���ϴ� ��. bEnableMultiHit�� true�� ��쿡�� �۵��ϴ� �ɼ�.
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = HitTrace)
	float DamageTickRate = 0.1;

	// ��Ƽ Ʈ���̽��� ����ϴ��� ���ϴ� ��. ����� �� �ѹ��� ��Ʈ�� �ټ��� ���Ͱ� ���Ե� �� ����
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
