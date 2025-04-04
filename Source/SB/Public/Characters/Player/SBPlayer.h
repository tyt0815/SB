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
class AWeapon;
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
	void PlayMontage(UAnimMontage* Montage);
	void PlayFireMontage();
	void PlayReloadMontage();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetLHIKTransform() const;

	UPROPERTY(BlueprintReadOnly, Category = States)
	ECharacterZoomState ZoomState;

	UPROPERTY(BlueprintReadOnly, Category = States)
	EEquippedWeapon EquippedWeaponType;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly, Category = Weapon)
	AWeapon* CurrentWeapon = nullptr;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void UseWeaponStarted();
	void UseWeaponOngoing();
	void UseWeaponCompleted();
	void UseWeaponSpecific1();
	void UseWeaponSpecific2();
	void Zoom();
	void ChangeWeapon1();
	void ChangeWeapon2();
	void Unarm();
	void EquipWeapon();
	void ZoomIn();
	void ZoomOut();

	/*
	* Enhanced Input
	*/
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* UseWeaponInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* UseWeaponSpecific1InputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* UseWeaponSpecific2InputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* ZoomInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* ChangeWeapon1InputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* ChangeWeapon2InputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* UnarmInputAction;

	/*
	* Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* RifleFireMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* RifleEquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* RifleReloadMontage;

	/*
	* SubClasses	
	*/
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AWeapon> RifleClass;

	FName RightHandSocketName = "hand_r_Socket";
	float JogScale = 1.0f;
	float WalkScale = 0.5f;
	float MovementSpeedScale;

public:
};
