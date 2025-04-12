// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Gun.h"
#include "Items/Bullet.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Player/SBPlayer.h"
#include "SB/DebugMacro.h"
# include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AGun::AGun()
{

}

void AGun::Tick(float Delta)
{
	Super::Tick(Delta);
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

void AGun::OnReloadEndNotify_Implementation()
{
	AmmoCount = MaxAmmo;
}

bool AGun::CanReload() const
{
	return !IsReloading() && AmmoCount < MaxAmmo;
}

bool AGun::IsReloading() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		return AnimInstance->Montage_IsPlaying(ReloadMontage);
	}
	return false;
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
	if (CanReload())
	{
		PlayMontage(ReloadMontage);
		ASBPlayer* Player = Cast<ASBPlayer>(GetOwner());
		if (Player)
		{
			Player->PlayReloadMontage(this);
		}
	}
	
}

FTransform AGun::CalcBulletSpawnTransform()
{
	FTransform Transform;
	Transform.SetLocation(GetMesh()->GetSocketLocation("Muzzle"));
	Transform.SetRotation(GetMesh()->GetSocketRotation("Muzzle").Quaternion());
	Transform.SetScale3D(FVector::OneVector);
	ASBPlayer* Player = Cast<ASBPlayer>(GetOwner());
	if (Player)
	{
		UCameraComponent* Camera = Player->GetCameraComponent();
		const FVector CameraLocation = Camera->GetComponentLocation();
		const FVector CameraForward = Camera->GetForwardVector();

		float SpreadAngleInRadians = FMath::DegreesToRadians(SpreadAngleInDegrees);
		const FVector ShootDirection = FMath::VRandCone(CameraForward, SpreadAngleInRadians);

		const FVector TraceStart = CameraLocation;
		const FVector TraceEnd = TraceStart + ShootDirection * 10000.0f;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		ActorsToIgnore.Add(Player);
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingle(
			this,
			TraceStart,
			TraceEnd,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);
		FVector TargetLocation = TraceEnd;
		if (HitResult.bBlockingHit)
		{
			TargetLocation = HitResult.ImpactPoint;
		}

		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Transform.GetLocation(), TargetLocation);
		Transform.SetRotation(Rotator.Quaternion());
	}
	return Transform;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	AmmoCount = MaxAmmo;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &AGun::Test);
	}
}

void AGun::Fire_Internal()
{
	FTransform Transform = CalcBulletSpawnTransform();
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(
		BulletClass,
		Transform.GetLocation(),
		Transform.GetRotation().Rotator()
	);
	if (Bullet)
	{
		Bullet->SetBulletSpeed(BulletSpeed);
		Bullet->SetOwner(this);
		++FireCount;
		--AmmoCount;
		ASBPlayer* Player = Cast<ASBPlayer>(GetOwner());
		if (Player)
		{
			Player->PlayFireMontage(this);
		}
	}
}

void AGun::SetFireReady(bool bReady)
{
	bFireReady = bReady;
}
