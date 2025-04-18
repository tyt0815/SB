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
class AWeapon;
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
	bool IsFireReady() const;

	UFUNCTION(BlueprintNativeEvent, Category = "BuildSystem")
	void OnPlayerPossessStarted();
	virtual void OnPlayerPossessStarted_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsUnarmed() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetLHIKTransform() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeapon* GetCurrentWeapon() const;

	UPROPERTY(BlueprintReadOnly, Category = States)
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	FVector ZoomOutCameraLoaction = FVector(500.0f, 0.0f, 120.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	FVector ZoomInCameraLoaction = FVector(200.0f, 70.0f, 70.0f);

	UPROPERTY(EditAnywhere, Category = Weapon)
	TArray<TSubclassOf<AWeapon>> WeaponClasses;

	UPROPERTY(EditAnywhere, Category = BuildSystem)
	TSubclassOf<ABuildCameraPawn> BuildCameraPawnClass;

	UPROPERTY(EditAnywhere, Category = BuildSystem)
	TSubclassOf<ABuildingCreater> BuildingCreaterClass;

	TArray<AWeapon*> WeaponQuickslot;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MouseLStarted();
	void MouseLOngoing();
	void MouseLCompleted();
	void RStarted();
	void BStarted();
	void MouseRStarted();
	void Number1Started();
	void Number2Started();
	void Number3Started();
	void SwitchWeapon(uint32 Index);
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

	/*
	* Enhanced Input
	*/
	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	TArray<UInputAction*> NumberInputActions;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MouseLInpuatAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* RInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* BInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* MouseRInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* TabInputAction;

	UPROPERTY(EditAnywhere, Category = EnhancedInput)
	UInputAction* CapsLockInputAction;

	/*
	* Montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TMap<EWeaponType, FWeaponMontageSet> WeaponMontages;

	/*
	* etc
	*/
	
	ABuildCameraPawn* BuildCameraPawn;
	ABuildingCreater* BuildingCreater;
	FName RightHandSocketName = "hand_r_Socket";
	float JogScale = 1.0f;
	float WalkScale = 0.4f;
	float MovementSpeedScale;
	uint16 CurrentWeaponIndex = 0;
	bool bUnArmed = true;
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
