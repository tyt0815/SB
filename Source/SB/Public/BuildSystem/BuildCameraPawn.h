// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BuildCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ASBPlayer;

UCLASS()
class SB_API ABuildCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ABuildCameraPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "BuildCamera")
	void OnPlayerPossessStarted();
	virtual void OnPlayerPossessStarted_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UFloatingPawnMovement* MovementComponent;

protected:
	void Move(const FInputActionValue& Value);
	void ToggleToPlayerCharacter();
	void SetInputMappingContext();

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* ToggleToPlayerCharacterInputAction;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	ASBPlayer* PlayerCharacter;

private:
	void InitializePlayerController();
	void TransferPlayerControllerPossessionToPawn(APawn* Pawn);

public:
};
