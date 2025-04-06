// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SB/DebugMacro.h"

ABullet::ABullet()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(BoxComponent);
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(BoxComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(BoxComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->Friction = 0.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 1.0f; // Set the lifespan of the bullet to 3 seconds
}

void ABullet::Tick(float Delta)
{
	Super::Tick(Delta);
	if (bEnableHitTrace)
	{
		EvaluateHitActors();
	}
	for (auto Actor : HitActors)
	{
		SCREEN_LOG_SINGLE_FRAME(Actor->GetName());
	}
	Location_LastFrame = GetActorLocation();
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	Location_LastFrame = GetActorLocation();
	InitializeActorsToIgnore();
}

void ABullet::SetBulletSpeed(float Speed)
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->InitialSpeed = Speed;
		ProjectileMovementComponent->MaxSpeed = Speed;
		ProjectileMovementComponent->Velocity = Speed * GetActorForwardVector();
	}
}

void ABullet::InitializeActorsToIgnore()
{
	ActorsToIgnore.Empty();
	ActorsToIgnore.Add(this);
	if (GetOwner())
	{
		ActorsToIgnore.Add(GetOwner());
	}
}

void ABullet::EvaluateHitActors()
{
	if (bEnableMultiTrace)
	{
		HitResults.Empty();
		UKismetSystemLibrary::BoxTraceMulti(
			this,
			CalculateHitTraceStartLocation(),
			CalculateHitTraceEndLocation(),
			CalculateHitTraceHalfSize(),
			GetActorRotation(),		
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame,
			HitResults,
			true
		);
	}
	else
	{
		HitResults.SetNum(1);
		UKismetSystemLibrary::BoxTraceSingle(
			this,
			CalculateHitTraceStartLocation(),
			CalculateHitTraceEndLocation(),
			CalculateHitTraceHalfSize(),
			GetActorRotation(),
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForOneFrame,
			HitResults[0],
			true
		);
	}

	for (const FHitResult& HitResult : HitResults)
	{
		if (HitResult.GetActor())
		{
			ActorsToIgnore.AddUnique(HitResult.GetActor());
			if (bEnableMultiHit)
			{
				// DamageTickRate초 후 ActorsToIgnore에서 다시 값을 제거하여 트레이스 가능하게 한다.
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, FName("RemoveFromActorsToIgnore"), HitResult.GetActor());
				GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, DamageTickRate, false);
			}
		}
	}
	
	HitActors.Empty();
	for (const FHitResult& HitResult : HitResults)
	{
		if (HitResult.GetActor())
		{
			HitActors.AddUnique(HitResult.GetActor());
		}
	}
}

FVector ABullet::CalculateHitTraceStartLocation()
{
	FVector Offset = Location_LastFrame;
	FVector ComponentForward = BoxComponent->GetForwardVector();
	float HalfWidth = BoxComponent->GetScaledBoxExtent().X;
	return Offset - ComponentForward * HalfWidth;
}

FVector ABullet::CalculateHitTraceEndLocation()
{
	FVector Offset = BoxComponent->GetComponentLocation();
	FVector ComponentForward = BoxComponent->GetForwardVector();
	float HalfWidth = BoxComponent->GetScaledBoxExtent().X;
	return Offset + ComponentForward * HalfWidth;
}

FVector ABullet::CalculateHitTraceHalfSize()
{
	FVector Extent = BoxComponent->GetScaledBoxExtent();
	return FVector(0.0f, Extent.Y, Extent.Z);
}

void ABullet::RemoveFromActorsToIgnore(AActor* Actor)
{
	ActorsToIgnore.Remove(Actor);
}