#include "Characters/Player/SBPlayer.h"

#include "BuildSystem/BuildCameraPawn.h"
#include "BuildSystem/BuildingCreater.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Items/Weapon.h"
#include "SB/DebugMacro.h"
#include "PlayerController/SBPlayerController.h"

ASBPlayer::ASBPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ�ѷ��� ȸ���� ĳ���Ͱ� ȸ������ �ʵ��� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	ZoomState = ECharacterZoomState::ECZS_NoZoom;
	MovementSpeedScale = JogScale;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);	// ȸ�� �ӵ�
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->MaxAcceleration = 1200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.5f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;
	

	// ������ �� ������Ʈ ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, ZoomOutCameraLoaction.Y, ZoomOutCameraLoaction.Z));
	CameraBoom->TargetArmLength = ZoomOutCameraLoaction.X;
	CameraBoom->bUsePawnControlRotation = true; // ī�޶� ��Ʈ�ѷ��� ���� ȸ���ǵ���	����

	// ī�޶� ������Ʈ ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// ī�޶� ������ �Ͽ� �����ǵ��� ����. USpringArmComponent::SocketName�� "SpringEndPoint"�� ������ �� ���κп� �����ǵ��� ����
	FollowCamera->bUsePawnControlRotation = false;	// ī�޶� ��Ʈ�ѷ��� ���� ȸ������ �ʵ��� ����

	// �迭 �ʱ�ȭ
	WeaponQuickslot.SetNum(2);
}

void ASBPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::Look);
		EnhancedInputComponent->BindAction(MouseRInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::MouseRTriggered);
		EnhancedInputComponent->BindAction(MouseLInpuatAction, ETriggerEvent::Triggered, this, &ASBPlayer::MouseLTriggered);
		EnhancedInputComponent->BindAction(MouseLInpuatAction, ETriggerEvent::Ongoing, this, &ASBPlayer::MouseLOngoing);
		EnhancedInputComponent->BindAction(MouseLInpuatAction, ETriggerEvent::Completed, this, &ASBPlayer::MouseLCompleted);
		EnhancedInputComponent->BindAction(RInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::RTriggered);
		EnhancedInputComponent->BindAction(BInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::BTriggered);
		EnhancedInputComponent->BindAction(TabInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::TabTriggered);
		EnhancedInputComponent->BindAction(CapsLockInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::CapsLockTriggered);
		if (NumberInputActions.IsValidIndex(1) && NumberInputActions[1])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[1], ETriggerEvent::Triggered, this, &ASBPlayer::Number1Triggered);
		}
		if (NumberInputActions.IsValidIndex(2) && NumberInputActions[2])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[2], ETriggerEvent::Triggered, this, &ASBPlayer::Number2Triggered);
		}
		if (NumberInputActions.IsValidIndex(3) && NumberInputActions[3])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[3], ETriggerEvent::Triggered, this, &ASBPlayer::Number3Triggered);
		}
	}
}

void ASBPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayerController();

	// Spawn Weapon
	UWorld* World = GetWorld();
	if(World)
	{
		FActorSpawnParameters WeaponSpawnParameters;
		WeaponSpawnParameters.Owner = this;
		if (WeaponClass0)
		{
			StockWeaponInQuickSlot(
				GetWorld()->SpawnActor<AWeapon>(WeaponClass0, WeaponSpawnParameters),
				0
			);
		}
		if (WeaponClass1)
		{
			StockWeaponInQuickSlot(
				GetWorld()->SpawnActor<AWeapon>(WeaponClass1, WeaponSpawnParameters),
				1
			);
		}

		SpawnBuildCameraPawn();
	}

	SpawnBuildingCreater();
}

void ASBPlayer::StockWeaponInQuickSlot(AWeapon* Weapon, uint32 Index)
{
	if (Weapon)
	{
		WeaponQuickslot[Index] = Weapon;
		SetWeaponVisibility(Weapon, false, false);
	}
}

void ASBPlayer::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void ASBPlayer::StopMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.2f);
	}
}

void ASBPlayer::PlayFireMontage(AWeapon* Weapon)
{
	if (Weapon)
	{
		EWeaponType WeaponType = Weapon->GetWeaponType();
		UAnimMontage* Montage = WeaponMontages[WeaponType].FireMontage;
		if (Montage)
		{
			
			PlayMontage(Montage);
		}
	}
	
}

void ASBPlayer::PlayReloadMontage(AWeapon* Weapon)
{
	if (Weapon)
	{
		EWeaponType WeaponType = Weapon->GetWeaponType();
		UAnimMontage* Montage = WeaponMontages[WeaponType].ReloadMontage;
		if (Montage)
		{
			UpperBodyState = EUpperBodyState::EUBS_Reloading;
			PlayMontage(Montage);
		}
	}
}

void ASBPlayer::PlayEquipMontage(AWeapon* Weapon)
{
	if (Weapon)
	{
		EWeaponType WeaponType = Weapon->GetWeaponType();
		UAnimMontage* Montage = WeaponMontages[WeaponType].EquipMontage;
		if (Montage)
		{
			UpperBodyState = EUpperBodyState::EUBS_Equipping;
			PlayMontage(Montage);
		}
	}
}

bool ASBPlayer::IsFireReady() const
{
	return !IsUnarmed() &&
		ZoomState == ECharacterZoomState::ECZS_Zooming &&
		GetUpperBodyState() == EUpperBodyState::EUBS_Idle
		;
}

void ASBPlayer::OnPlayerPossessStarted_Implementation()
{
	InitializePlayerController();
}

bool ASBPlayer::IsUnarmed() const
{
	return bUnArmed || GetCurrentWeapon() == nullptr;
}

// TODO: �̰� �� �ٲٱ� �ؾ��� �� ����.
FTransform ASBPlayer::GetLHIKTransform() const
{
	FTransform LHIKTransform;
	AWeapon* Weapon = GetCurrentWeapon();
	if (Weapon != nullptr)
	{
		FTransform Transform = Weapon->GetMesh()->GetSocketTransform(FName("LHIK"));
		FVector Location;
		FRotator Rotator;
		GetMesh()->TransformToBoneSpace("hand_r", Transform.GetLocation(), Transform.GetRotation().Rotator(), Location, Rotator);
		LHIKTransform.SetLocation(Location);
		LHIKTransform.SetRotation(Rotator.Quaternion());
	}
	return LHIKTransform;
}

AWeapon* ASBPlayer::GetCurrentWeapon() const
{
	if (WeaponQuickslot.IsValidIndex(CurrentWeaponIndex))
	{
		return WeaponQuickslot[CurrentWeaponIndex];
	}
	else
	{
		return nullptr;
	}
}

void ASBPlayer::OnReloadEndNotify_Implementation()
{
	ReloadEnd();
}

void ASBPlayer::OnEquipEndNotify_Implementation()
{
	EquipEnd();
}

void ASBPlayer::ZoomIn_Implementation()
{
	ZoomState = ECharacterZoomState::ECZS_Zooming;
	MovementSpeedScale = WalkScale;
}

void ASBPlayer::ZoomOut_Implementation()
{
	ZoomState = ECharacterZoomState::ECZS_NoZoom;
	MovementSpeedScale = JogScale;
}


void ASBPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y * MovementSpeedScale);
		AddMovementInput(RightDirection, MovementVector.X * MovementSpeedScale);
	}
}

void ASBPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASBPlayer::MouseLTriggered()
{
	if (IsFireReady()) 
	{
		GetCurrentWeapon()->UseStart();
	}
}

void ASBPlayer::MouseLOngoing()
{
	if (IsFireReady())
	{
		GetCurrentWeapon()->UseOngoing();
	}
}

void ASBPlayer::MouseLCompleted()
{
	if (IsFireReady())
	{
		GetCurrentWeapon()->UseStop();
	}
}

void ASBPlayer::RTriggered()
{
	if (!IsUnarmed() && GetUpperBodyState() == EUpperBodyState::EUBS_Idle)
	{
		GetCurrentWeapon()->SpecificUse1();
	}
}

void ASBPlayer::BTriggered()
{
	if (!IsUnarmed() && GetUpperBodyState() == EUpperBodyState::EUBS_Idle)
	{
		GetCurrentWeapon()->SpecificUse2();
	}
}

void ASBPlayer::MouseRTriggered()
{
	if (IsUnarmed() || ZoomState == ECharacterZoomState::ECZS_Zooming)
	{
		ZoomOut();
			
	}
	else if (ZoomState == ECharacterZoomState::ECZS_NoZoom)
	{
		ZoomIn();
	}
	else
	{
		SCREEN_LOG_NONE_KEY("Undefined ZoomState: ASBPlayer::Zoom");
	}
}

void ASBPlayer::Number1Triggered()
{
	SwitchWeapon(0);
}

void ASBPlayer::Number2Triggered()
{
	SwitchWeapon(1);
}

void ASBPlayer::Number3Triggered()
{
	UnequipWeapon();
	ZoomOut();
}

void ASBPlayer::SwitchWeapon(uint32 Index)
{
	if (CurrentWeaponIndex != Index)
	{
		UnequipWeapon();
		EquipWeapon(Index);
		PlayEquipMontage(GetCurrentWeapon());
	}
}

void ASBPlayer::EquipWeapon(uint32 Index)
{
	bUnArmed = false;
	SetCurrentWeapon(Index);
	AttachWeapon(GetCurrentWeapon(), RightHandSocketName);
	SetWeaponVisibility(GetCurrentWeapon(), true, true);
}

void ASBPlayer::UnequipWeapon()
{
	bUnArmed = true;
	DettachWeapon(GetCurrentWeapon());
	SetWeaponVisibility(GetCurrentWeapon(), false, true);
	StopMontage();
	if (GetCurrentWeapon())
	{
		GetCurrentWeapon()->StopMontage();
	}
	CurrentWeaponIndex = 3;
}

void ASBPlayer::SetWeaponVisibility(AWeapon* Weapon, bool bVisibility, bool bEffect)
{
	if (Weapon)
	{
		if (bEffect)
		{
			Weapon->SetMeshVisibleWithEffect(bVisibility);
		}
		else
		{
			Weapon->GetMesh()->SetVisibility(bVisibility);
		}
	}
}

void ASBPlayer::SetCurrentWeapon(uint32 Index)
{
	if (WeaponQuickslot.IsValidIndex(Index))
	{
		CurrentWeaponIndex = Index;
	}
}

void ASBPlayer::AttachWeapon(AWeapon* Weapon, FName SocketName)
{
	if (Weapon)
	{
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		Weapon->GetMesh()->AttachToComponent(GetMesh(), AttachmentTransformRules, SocketName);
	}
}

void ASBPlayer::DettachWeapon(AWeapon* Weapon)
{
	if (Weapon)
	{
		Weapon->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void ASBPlayer::ReloadEnd()
{
	UpperBodyState = EUpperBodyState::EUBS_Idle;
}

void ASBPlayer::EquipEnd()
{
	UpperBodyState = EUpperBodyState::EUBS_Idle;
}

void ASBPlayer::TabTriggered()
{
	if (ControllMode == ECharacterControllMode::ECCM_Build)
	{
		ControllMode = ECharacterControllMode::ECCM_Combat;
	}
	else
	{
		ControllMode = ECharacterControllMode::ECCM_Build;
	}
}

void ASBPlayer::CapsLockTriggered()
{
	if (ControllMode == ECharacterControllMode::ECCM_Build)
	{
		if (BuildCameraPawn == nullptr)
		{
			SpawnBuildCameraPawn();
		}
		if (BuildCameraPawn)
		{
			TransferPlayerControllerPossessionToPawn(BuildCameraPawn);
			BuildCameraPawn->OnPlayerPossessStarted();
			GetCharacterMovement()->StopMovementImmediately();
		}
	}
}

bool ASBPlayer::IsPlayingMontage(UAnimMontage* Montage) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		return AnimInstance->Montage_IsPlaying(Montage);
	}
	return false;
}

bool ASBPlayer::IsPlayingFireMontage(AWeapon* Weapon) const
{
	if (Weapon)
	{
		UAnimMontage* Montage = WeaponMontages[Weapon->GetWeaponType()].FireMontage;
		return IsPlayingMontage(Montage);
	}
	return false;
}

bool ASBPlayer::IsPlayingEquipMontage(AWeapon* Weapon) const
{
	if (Weapon)
	{
		UAnimMontage* Montage = WeaponMontages[Weapon->GetWeaponType()].EquipMontage;
		return IsPlayingMontage(Montage);
	}
	return false;
}

bool ASBPlayer::IsPlayingReloadMontage(AWeapon* Weapon) const
{
	if (Weapon)
	{
		UAnimMontage* Montage = WeaponMontages[Weapon->GetWeaponType()].ReloadMontage;
		return IsPlayingMontage(Montage);
	}
	return false;
}

void ASBPlayer::InitializePlayerController()
{
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->SetMouseInterface(false);
		PlayerController->PlayerCameraManager->ViewPitchMax = 70.0f;
		PlayerController->PlayerCameraManager->ViewPitchMin = -70.0f;
		PlayerController->AddInputMappingContext(DefaultMappingContext);
	}
}

void ASBPlayer::TransferPlayerControllerPossessionToPawn(APawn* Pawn)
{
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->RemoveInputMappingContext(DefaultMappingContext);
		PlayerController->UnPossess();
		PlayerController->Possess(Pawn);
	}
}

void ASBPlayer::SpawnBuildCameraPawn()
{
	// Spawn BuildCameraPawn
	if (BuildCameraPawnClass)
	{
		FActorSpawnParameters BuildCameraSpawnParameters;
		BuildCameraSpawnParameters.Owner = this;
		BuildCameraPawn = GetWorld()->SpawnActor<ABuildCameraPawn>(BuildCameraPawnClass, BuildCameraSpawnParameters);
	}
	if (BuildCameraPawn == nullptr)
	{
		SCREEN_LOG_NONE_KEY("ABuildCameraPawn spawn failed.");
	}
}

void ASBPlayer::SpawnBuildingCreater()
{
	UWorld *World = GetWorld();
	if (World && BuildingCreaterClass)
	{
		BuildingCreater = World->SpawnActor<ABuildingCreater>(BuildingCreaterClass);
		if (BuildingCreater)
		{
			BuildingCreater->SetGridVisibility(false);
		}
	}
}
