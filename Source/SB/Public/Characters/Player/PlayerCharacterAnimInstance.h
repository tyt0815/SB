// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SB_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadOnly, Category = CharacterProperty)
	class APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = CharacterProperty)
	class UCharacterMovementComponent* PlayerMovementComponent;	

	UPROPERTY(BlueprintReadOnly, Category = CharacterProperty)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = CharacterProperty)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = CharacterProperty)
	bool ShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = CharacterProperty)
	bool IsFalling;
};
