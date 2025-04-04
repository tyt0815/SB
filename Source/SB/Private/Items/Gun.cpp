// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Gun.h"
#include "Items/Bullet.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Player/SBPlayer.h"
#include "SB/DebugMacro.h"

AGun::AGun()
{
	UArrowComponent* ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	SetRootComponent(ArrowComponent);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollisionComponent->SetupAttachment(SkeletalMeshComponent);
}

void AGun::UseStart()
{
	FireCount = 0;
	Fire();
}

void AGun::UseOngoing()
{
	if (FireMode == EFireMode::EFM_FullAuto)
	{
		Fire();
	}
}

void AGun::SpecificUse1()
{
	Reload();
}

void AGun::SpecificUse2()
{
	if (FireMode == EFireMode::EFM_FullAuto)
	{
		SCREEN_LOG_NONE_KEY("Change Fire Mode to Burst");
		FireMode = EFireMode::EFM_Burst;
	}
	else if (FireMode == EFireMode::EFM_Burst)
	{
		SCREEN_LOG_NONE_KEY("Change Fire Mode to SemiAuto");
		FireMode = EFireMode::EFM_SemiAuto;
	}
	else
	{
		SCREEN_LOG_NONE_KEY("Change Fire Mode to FullAuto");
		FireMode = EFireMode::EFM_FullAuto;
	}
}

void AGun::OnReloadEnd_Implementation()
{
	SCREEN_LOG_NONE_KEY("OnGunReloadEnd_Implementation");
}

void AGun::Test(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	SCREEN_LOG_NONE_KEY(NotifyName.ToString());
}

bool AGun::CanFire()
{
	return bFireReady && AmmoCount > 0;
}

void AGun::StartFireCooldownTimer(float CoolDownTime)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("SetFireReady"), true);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, CoolDownTime, false);
}

void AGun::Fire()
{
	if (CanFire() && BulletClass)
	{
		Fire_Internal();
		bFireReady = false;
		if (FireMode == EFireMode::EFM_Burst)
		{
			for (int i = 1; i < BurstNum; ++i)
			{
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, FName("Fire_Internal"));
				GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, FireRate * i, false);
			}
			StartFireCooldownTimer(FireRate * BurstNum);
		}
		else
		{
			StartFireCooldownTimer(FireRate);
		}
	}
}

void AGun::Reload()
{
	PlayMontage(ReloadMontage);
	ASBPlayer* Player = Cast<ASBPlayer>(GetOwner());
	if (Player)
	{
		Player->PlayReloadMontage();
	}
	AmmoCount = 30;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	AmmoCount = MaxAmmo;

	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (AnimInstance)
	{
		SCREEN_LOG_NONE_KEY("AnimInstance is not null");
		AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &AGun::Test);
	}
}

void AGun::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		
		AnimInstance->Montage_Play(Montage);
	}
}

void AGun::Fire_Internal()
{
	SCREEN_LOG_NONE_KEY("Fire_Internal");
	FVector SpawnLocation = SkeletalMeshComponent->GetSocketLocation("Muzzle");
	FRotator SpawnRotation = SkeletalMeshComponent->GetSocketRotation("Muzzle");
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation);
	if (Bullet)
	{
		Bullet->SetBulletSpeed(BulletSpeed);
		Bullet->SetOwner(this);
		++FireCount;
		--AmmoCount;
		ASBPlayer* Player = Cast<ASBPlayer>(GetOwner());
		if (Player)
		{
			Player->PlayFireMontage();
		}
	}
}

void AGun::SetFireReady(bool bReady)
{
	bFireReady = bReady;
}
