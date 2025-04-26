// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SBCharacter.h"
#include "CharacterType.h"
#include "Items/ItemType.h"
#include "Items/GunAnimNotify.h"
#include "SBPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UInteractionComponent;
class AItem;
class AWeapon;
class ABuilding;
class ABuildCameraPawn;
class ABuildingCreater;
struct FInputActionValue;

USTRUCT(BlueprintType)
struct FWeaponMontageSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* ReloadMontage;
};

UCLASS()
class SB_API ASBPlayer : public ASBCharacter, public IGunAnimNotify
{
	GENERATED_BODY()
public:
	ASBPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
public:
	void StockWeaponInQuickSlot(AWeapon* Weapon, uint32 Index);
	void PlayMontage(UAnimMontage* Montage);
	void StopMontage();
	void PlayFireMontage(AWeapon* Weapon);
	void PlayReloadMontage(AWeapon* Weapon);
	void PlayEquipMontage(AWeapon* Weapon);
	bool PickUpItem(AItem* Item);
	bool IsFireReady() const;
	TArray<TSubclassOf<ABuilding>> GetBuildingList();

	UFUNCTION(BlueprintNativeEvent, Category = SBPlayer)
	void OnPlayerPossessStarted();
	virtual void OnPlayerPossessStarted_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsUnarmed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetLHIKTransform() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeapon* GetCurrentWeapon() const;

	UPROPERTY(BlueprintReadOnly, Category = SBPlayer)
	ECharacterZoomState ZoomState;

	// GunAnimNotify Functions
	virtual void OnReloadEndNotify_Implementation() override;
	virtual void OnEquipEndNotify_Implementation() override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void ZoomIn();
	virtual void ZoomIn_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void ZoomOut();
	virtual void ZoomOut_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInventoryComponent* Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SBPlayer)
	FVector ZoomOutCameraLoaction = FVector(500.0f, 0.0f, 120.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SBPlayer)
	FVector ZoomInCameraLoaction = FVector(200.0f, 70.0f, 70.0f);

	UPROPERTY(EditAnywhere, Category = SBPlayer)
	TArray<TSubclassOf<AWeapon>> WeaponClasses;

	UPROPERTY(EditAnywhere, Category = SBPlayer)
	TSubclassOf<ABuildCameraPawn> BuildCameraPawnClass;

	UPROPERTY(EditAnywhere, Category = SBPlayer)
	TSubclassOf<ABuildingCreater> BuildingCreaterClass;

	UPROPERTY(EditAnywhere, Category = SBPlayer)
	TArray<TSubclassOf<ABuilding>> BuildingClasses;

	TArray<AWeapon*> WeaponQuickslot;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MouseLStarted();
	void MouseLOngoing();
	void MouseLCompleted();
	void MouseRStarted();
	void MouseWheelStarted(const FInputActionValue& Value);
	void RStarted();
	void BStarted();
	void IStarted();
	void NumberKeysStarted(uint32 i);
	void Number1Started();
	void Number2Started();
	void Number3Started();
	void SwitchWeapon(uint32 Index);
	void SwitchToUnarmedState();
	void SwitchBuildPreviewMesh(uint32 Index);
	void EquipWeapon(uint32 Index);
	void UnequipWeapon();
	void SetWeaponVisibility(AWeapon* Weapon, bool bVisibility, bool bEffect);
	void SetCurrentWeapon(uint32 Index);
	void AttachWeapon(AWeapon* Weapon, FName SocketName);
	void DettachWeapon(AWeapon* Weapon);
	void ReloadEnd();
	void EquipEnd();
	void TabStarted();
	void CapsLockStarted();
	bool IsPlayingMontage(UAnimMontage* Montage) const;
	bool IsPlayingFireMontage(AWeapon* Weapon) const;
	bool IsPlayingEquipMontage(AWeapon* Weapon) const;
	bool IsPlayingReloadMontage(AWeapon* Weapon) const;
	void InitializePlayerController();
	void TransferPlayerControllerPossessionToPawn(APawn* Pawn);
	void SpawnBuildCameraPawn();
	void SpawnBuildingCreater();
	void SpawnAndStockWeapon(uint32 i);
	void SetBuildingCreaterLocation();
	void ConvertToUIUseMode(bool bUse);
	void TraceInteractionActors();
	void SelectInteractionActor();
	float ForwardVectorDot(FVector B);

	/*
	* Enhanced Input
	*/
	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	TArray<UInputAction*> NumberInputActions;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* MouseLInpuatAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* MouseRInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* MouseWheelInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* RInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* BInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* IInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* TabInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | EnhancedInput")
	UInputAction* CapsLockInputAction;

	UPROPERTY(EditAnywhere, Category = "SBPlayer | Interaction")
	float InteractionRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = "SBPlayer | Montages")
	TMap<EWeaponType, FWeaponMontageSet> WeaponMontages;
	
	TArray<AActor*> InteractiveActorList;
	AActor* FocusedInteractable = nullptr;
	UInteractionComponent* FocusedInteractionComponent = nullptr;
	int32 FocusedInteractionOptionIndex = 0;
	ABuildCameraPawn* BuildCameraPawn;
	ABuildingCreater* BuildingCreater;
	class ASBHUD* HUD;
	class USBPlayerOverlayWidget* OverlayWidget;
	FName RightHandSocketName = "hand_r_Socket";
	float JogScale = 1.0f;
	float WalkScale = 0.4f;
	float MovementSpeedScale;
	uint16 CurrentWeaponIndex = 0;
	bool bUnArmed = true;
	bool bUseUI = false;
	EUpperBodyState UpperBodyState = EUpperBodyState::EUBS_Idle;
	ECharacterControllMode ControllMode = ECharacterControllMode::ECCM_Combat;

public:
	FORCEINLINE UCameraComponent* GetCameraComponent() const
	{
		return FollowCamera;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EUpperBodyState GetUpperBodyState() const
	{
		return UpperBodyState;
	}
};
