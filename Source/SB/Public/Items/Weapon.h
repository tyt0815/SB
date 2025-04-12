// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Items/ItemType.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class SB_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

public:
	void PlayMontage(UAnimMontage* Montage);
	void StopMontage();
	virtual void UseStart() {};
	virtual void UseOngoing() {};
	virtual void UseStop() {};
	virtual void SpecificUse1() {};
	virtual void SpecificUse2() {};

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void SetMeshVisibleWithEffect(bool bVisible);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;

public:
	FORCEINLINE USkeletalMeshComponent* GetMesh() const
	{
		return Mesh;
	}
	FORCEINLINE EWeaponType GetWeaponType() const
	{
		return WeaponType;
	}
};
