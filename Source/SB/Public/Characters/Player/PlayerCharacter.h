// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class SB_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MoveAction;

private:

public:	

};
