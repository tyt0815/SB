// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterType.h"
#include "Items/ItemType.h"
#include "SBPlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8
{
	ERYOM_Accumulate UMETA(DisplayName = "Accumulate"),
	ERYOM_BlendOut UMETA(DisplayName = "BlendOut"),
	ERYOM_Hold UMETA(DisplayName = "Hold")
};

UCLASS()
class SB_API USBPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = References)
	class ASBPlayer* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = References)
	class UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Essential Values")
	float Acceleration;

	UPROPERTY(BlueprintReadWrite, Category = "Essential Values")
	float LastFrameAcceleration;

	UPROPERTY(BlueprintReadWrite, Category = "Essential Values")
	float Speed;

	UPROPERTY(BlueprintReadWrite, Category = "Essential Values")
	float YawDelta;

	UPROPERTY(BlueprintReadWrite, Category = "Essential Values")
	FRotator WorldRotator;

	UPROPERTY(BlueprintReadWrite, Category = "States")
	ECharacterMovementMode MovementMode;

	UPROPERTY(BlueprintReadWrite, Category = "States")
	ECharacterMovementState MovementState;

	UPROPERTY(BlueprintReadWrite, Category = "States")
	ECharacterZoomState ZoomState;

	UPROPERTY(BlueprintReadWrite, Category = "States")
	EWeaponType CurrentWeaponType;

	UPROPERTY(BlueprintReadWrite, Category = "States")
	bool bUnarmed;

	UPROPERTY(BlueprintReadWrite, Category = "Aiming Data")
	float AimPitch;

	UPROPERTY(BlueprintReadWrite, Category = "Aiming Data")
	float AimYaw;

	UPROPERTY(BlueprintReadWrite, Category = "Turn In Place Data")
	float RootYawOffset;

	UPROPERTY(BlueprintReadWrite, Category = "Turn In Place Data")
	ERootYawOffsetMode RootYawOffsetMode;
	

private:
};
