// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"

AWeapon::AWeapon()
{
	
}

void AWeapon::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void AWeapon::StopMontage()
{
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.2f);
	}
}
