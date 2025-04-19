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
class ABuilding;

UENUM(BlueprintType)
enum class EBuildMode : uint8
{
	EBM_Interaction UMETA(DisplayName = "Interaction"),
	EBM_Placement UMETA(DisplayName = "Placement")
};

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
	void MouseLTriggered();
	void MouseRTriggered();
	void EndPlacementMode();
	void Num1Triggerd();
	void SelectBuilding(ABuilding* Building);
	void DeselectBuilding();
	void SetInputMappingContext();
	void TraceUnderMouseCursor();

	// �Է� Building�� nullptr�̾ �ȵȴ�.
	FVector CalculatePlacementLocation(ABuilding* Building);

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* ToggleToPlayerCharacterInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MouseLInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MouseRInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* Num1InputAction;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	ASBPlayer* PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, Category = "States")
	EBuildMode BuildMode = EBuildMode::EBM_Interaction;

	UPROPERTY(EditAnywhere, Category = "Subclasses")
	TArray<TSubclassOf<ABuilding>> PreviewBuildingClasses;

private:
	void InitializePlayerController();
	void TransferPlayerControllerPossessionToPawn(APawn* Pawn);
	void GetMouseWorldPosition(FVector& WorldLocation, FVector& WorldDirection);
	void ChangeMouseHoveredBuilding();
	void ChangeSelectedBuilding(ABuilding* Building);
	void ChangePreviewBuilding(int i);
	void SwitchToPlacementMode(int PreviewBuildingIndex);

	ABuilding* MouseHoveredBuilding;
	ABuilding* SelectedBuilding;
	TArray<ABuilding*> PreviewBuildings;
	FHitResult MouseDownTraceHit;
	int CurrentPreviewBuildingIndex = 0;

public:
	FORCEINLINE ABuilding* GetCurrentPreviewBuilding() const
	{
		return PreviewBuildings[CurrentPreviewBuildingIndex];
	}
};
