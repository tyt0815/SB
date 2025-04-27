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
enum class ECharacterZoomState : uint8
{
	ECZS_NoZoom UMETA(DisplayName = "NoZoom"),
	ECZS_Zooming UMETA(DisplayName = "Zooming")
};

UENUM(BlueprintType)
enum class EUpperBodyState : uint8
{
	EUBS_Idle UMETA(DisplayName = "Idle"),
	EUBS_Reloading UMETA(DisplayName = "Reloading"),
	EUBS_Equipping UMETA(DisplayName = "Equipping")
};

UENUM(BlueprintType)
enum class ECharacterControllMode : uint8
{
	ECCM_Combat UMETA(DisplayName = "Combat"),
	ECCM_Build UMETA(DisplayName = "Build"),
	ECCM_None UMETA(DisplayName = "None")
};