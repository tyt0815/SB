// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SBCharacter.h"
#include "CharacterType.h"
#include "SBPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class SB_API ASBPlayer : public ASBCharacter
{
	GENERATED_BODY()
public:
	ASBPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = States)
	ECharacterZoomState ZoomState;

	UPROPERTY(BlueprintReadOnly, Category = States)
	EEquippedWeapon EquippedWeaponType;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void ChangeWeapon1(const FInputActionValue& Value);
	void ChangeWeapon2(const FInputActionValue& Value);
	void Unarm(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* ChangeWeapon1Action;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* ChangeWeapon2Action;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* UnarmAction;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* RifleFireMontage;

	float JogScale = 1.0f;
	float WalkScale = 0.5f;
	float MovementSpeedScale;

private:
	

public:
};
