#pragma once

UENUM(BlueprintType)
enum class ECharacterMovementMode : uint8
{
	ECMM_OnGround UMETA(DisplayName = "OnGround"),
	ECMM_InAir UMETA(DisplayName = "InAir"),
};

UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	ECMS_Idle UMETA(DisplayName = "Idle"),
	ECMS_Moving UMETA(DisplayName = "Moving"),
};

UENUM(BlueprintType)
enum class EEquippedWeapon : uint8
{
	EEW_Unarmed UMETA(DisplayName = "Unarmed"),
	EEW_Rifle UMETA(DisplayName = "Rifle")
};

UENUM(BlueprintType)
enum class ECharacterZoomState : uint8
{
	ECZS_NoZoom UMETA(DisplayName = "NoZoom"),
	ECZS_Zooming UMETA(DisplayName = "Zooming")
};