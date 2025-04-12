// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon.h"
#include "Items/GunAnimNotify.h"
#include "Gun.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	EFM_FullAuto UMETA(DisplayName = "FullAuto"),
	EFM_Burst UMETA(DisplayName = "Burst"),
	EFM_SemiAuto UMETA(DisplayName = "SemiAuto")
};

UCLASS()
class SB_API AGun : public AWeapon, public IGunAnimNotify
{
	GENERATED_BODY()
	
public:
	AGun();
	virtual void Tick(float Delta) override;

protected:
	virtual void BeginPlay() override;
public:
	void PlayMontage(UAnimMontage* Montage);
	virtual void UseStart() override;
	virtual void UseOngoing() override;
	// Reload
	virtual void SpecificUse1() override;
	// Change Fire Mode
	virtual void SpecificUse2() override;

	// GunAnimNotify Functions
	virtual void OnReloadEndNotify_Implementation() override;

	bool CanReload() const;

	void Test(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gun")
	TSubclassOf<class ABullet> BulletClass;

	/*
	* Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	float BulletSpeed = 3000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	float FireRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	float Damage = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	int32 MaxAmmo = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	int32 BurstNum = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	float SpreadAngle = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Gun")
	EFireMode FireMode = EFireMode::EFM_SemiAuto;

private:
	void StartFireCooldownTimer(float CoolDownTime);
	void Fire();
	void Reload();
	FTransform CalcBulletSpawnTransform();
	bool CanFire();

	UFUNCTION()
	void Fire_Internal();

	UFUNCTION()
	void SetFireReady(bool bReady);

	int32 AmmoCount = 0;
	int16 FireCount = 0;
	bool bFireReady = true;
	bool bReloading = false;
};
