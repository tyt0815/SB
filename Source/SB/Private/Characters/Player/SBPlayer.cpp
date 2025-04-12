#include "Characters/Player/SBPlayer.h"

#include "BuildSystem/BuildCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Items/Weapon.h"
#include "SB/DebugMacro.h"

ASBPlayer::ASBPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 컨트롤러의 회전에 캐릭터가 회전하지 않도록 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	ZoomState = ECharacterZoomState::ECZS_NoZoom;
	MovementSpeedScale = JogScale;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);	// 회전 속도
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->MaxAcceleration = 1200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.5f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;
	

	// 스프링 암 컴포넌트 생성
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, ZoomOutCameraLoaction.Y, ZoomOutCameraLoaction.Z));
	CameraBoom->TargetArmLength = ZoomOutCameraLoaction.X;
	CameraBoom->bUsePawnControlRotation = true; // 카메라가 컨트롤러에 의해 회전되도록	설정

	// 카메라 컴포넌트 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// 카메라가 스프링 암에 부착되도록 설정. USpringArmComponent::SocketName는 "SpringEndPoint"로 스프링 암 끝부분에 부착되도록 설정
	FollowCamera->bUsePawnControlRotation = false;	// 카메라가 컨트롤러에 의해 회전되지 않도록 설정

	// 배열 초기화
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
		EnhancedInputComponent->BindAction(ZoomInputAction, ETriggerEvent::Started, this, &ASBPlayer::Zoom);
		EnhancedInputComponent->BindAction(UseWeaponInputAction, ETriggerEvent::Started, this, &ASBPlayer::UseWeaponStarted);
		EnhancedInputComponent->BindAction(UseWeaponInputAction, ETriggerEvent::Ongoing, this, &ASBPlayer::UseWeaponOngoing);
		EnhancedInputComponent->BindAction(UseWeaponInputAction, ETriggerEvent::Completed, this, &ASBPlayer::UseWeaponCompleted);
		EnhancedInputComponent->BindAction(UseWeaponSpecific1InputAction, ETriggerEvent::Started, this, &ASBPlayer::UseWeaponSpecific1);
		EnhancedInputComponent->BindAction(UseWeaponSpecific2InputAction, ETriggerEvent::Started, this, &ASBPlayer::UseWeaponSpecific2);
		EnhancedInputComponent->BindAction(SwitchWeapon0InputAction, ETriggerEvent::Started, this, &ASBPlayer::SwitchToWeapon0);
		EnhancedInputComponent->BindAction(SwitchWeapon1InputAction, ETriggerEvent::Started, this, &ASBPlayer::SwitchToWeapon1);
		EnhancedInputComponent->BindAction(SwitchToUnarmInputAction, ETriggerEvent::Started, this, &ASBPlayer::SwitchToUnarmedState);
		EnhancedInputComponent->BindAction(ToggleToBuildModeInputAction, ETriggerEvent::Started, this, &ASBPlayer::ToggleToBuildModeStarted);
	}
}

void ASBPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetInputMappingContext();

	// Spawn Weapon
	UWorld* World = GetWorld();
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
	}

	// Spawn BuildCameraPawn
	FActorSpawnParameters BuildCameraSpawnParameters;
	if (BuildCameraPawnClass)
	{
		BuildCameraPawn = GetWorld()->SpawnActor<ABuildCameraPawn>(BuildCameraPawnClass, BuildCameraSpawnParameters);
		BuildCameraPawn->SetPlayerCharacter(this);
	}
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
	SetInputMappingContext();
}

bool ASBPlayer::IsUnarmed() const
{
	return bUnArmed || GetCurrentWeapon() == nullptr;
}

// TODO: 이거 좀 바꾸긴 해야할 거 같음.
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

void ASBPlayer::UseWeaponStarted()
{
	if (IsFireReady()) 
	{
		GetCurrentWeapon()->UseStart();
	}
}

void ASBPlayer::UseWeaponOngoing()
{
	if (IsFireReady())
	{
		GetCurrentWeapon()->UseOngoing();
	}
}

void ASBPlayer::UseWeaponCompleted()
{
	if (IsFireReady())
	{
		GetCurrentWeapon()->UseStop();
	}
}

void ASBPlayer::UseWeaponSpecific1()
{
	if (!IsUnarmed() && GetUpperBodyState() == EUpperBodyState::EUBS_Idle)
	{
		GetCurrentWeapon()->SpecificUse1();
	}
}

void ASBPlayer::UseWeaponSpecific2()
{
	if (!IsUnarmed() && GetUpperBodyState() == EUpperBodyState::EUBS_Idle)
	{
		GetCurrentWeapon()->SpecificUse2();
	}
}

void ASBPlayer::Zoom()
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

void ASBPlayer::SwitchToWeapon0()
{
	SwitchWeapon(0);
}

void ASBPlayer::SwitchToWeapon1()
{
	SwitchWeapon(1);
}

void ASBPlayer::SwitchToUnarmedState()
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

void ASBPlayer::ToggleToBuildModeStarted()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		check(BuildCameraPawn);
		RemoveInputMappingContext();
		PlayerController->UnPossess();
		PlayerController->Possess(BuildCameraPawn);
		BuildCameraPawn->OnPlayerPossessStarted();
		GetCharacterMovement()->StopMovementImmediately();
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

void ASBPlayer::SetInputMappingContext()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		PlayerController->PlayerCameraManager->ViewPitchMax = 70.0f;
		PlayerController->PlayerCameraManager->ViewPitchMin = -70.0f;
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASBPlayer::RemoveInputMappingContext()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->RemoveMappingContext(DefaultMappingContext);
		}
	}
}
