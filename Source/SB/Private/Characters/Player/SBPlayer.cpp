// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SBPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SB/DebugMacro.h"
#include "Items/Weapon.h"

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
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true; // 카메라가 컨트롤러에 의해 회전되도록	설정

	// 카메라 컴포넌트 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// 카메라가 스프링 암에 부착되도록 설정. USpringArmComponent::SocketName는 "SpringEndPoint"로 스프링 암 끝부분에 부착되도록 설정
	FollowCamera->bUsePawnControlRotation = false;	// 카메라가 컨트롤러에 의해 회전되지 않도록 설정
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
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ASBPlayer::Jump);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ASBPlayer::StopJumping);
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ASBPlayer::Look);
		EnhancedInputComponent->BindAction(ZoomInputAction, ETriggerEvent::Started, this, &ASBPlayer::Zoom);
		EnhancedInputComponent->BindAction(UseWeaponInputAction, ETriggerEvent::Started, this, &ASBPlayer::UseWeaponStarted);
		EnhancedInputComponent->BindAction(UseWeaponInputAction, ETriggerEvent::Ongoing, this, &ASBPlayer::UseWeaponOngoing);
		EnhancedInputComponent->BindAction(UseWeaponInputAction, ETriggerEvent::Completed, this, &ASBPlayer::UseWeaponCompleted);
		EnhancedInputComponent->BindAction(UseWeaponSpecific1InputAction, ETriggerEvent::Started, this, &ASBPlayer::UseWeaponSpecific1);
		EnhancedInputComponent->BindAction(UseWeaponSpecific2InputAction, ETriggerEvent::Started, this, &ASBPlayer::UseWeaponSpecific2);
		EnhancedInputComponent->BindAction(ChangeWeapon1InputAction, ETriggerEvent::Started, this, &ASBPlayer::ChangeWeapon1);
		EnhancedInputComponent->BindAction(ChangeWeapon2InputAction, ETriggerEvent::Started, this, &ASBPlayer::ChangeWeapon2);
		EnhancedInputComponent->BindAction(UnarmInputAction, ETriggerEvent::Started, this, &ASBPlayer::Unarm);
	}
}

void ASBPlayer::BeginPlay()
{
	Super::BeginPlay();

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

void ASBPlayer::PlayFireMontage()
{
	if (EquippedWeaponType == EEquippedWeapon::EEW_Rifle)
	{
		PlayMontage(RifleFireMontage);
	}
}

void ASBPlayer::PlayReloadMontage()
{
	if (EquippedWeaponType == EEquippedWeapon::EEW_Rifle)
	{
		PlayMontage(RifleReloadMontage);
	}
}

FTransform ASBPlayer::GetLHIKTransform() const
{
	FTransform LHIKTransform;
	if (CurrentWeapon != nullptr)
	{
		FTransform Transform = CurrentWeapon->GetSkeletalMeshComponent()->GetSocketTransform(FName("LHIK"));
		FVector Location;
		FRotator Rotator;
		GetMesh()->TransformToBoneSpace("hand_r", Transform.GetLocation(), Transform.GetRotation().Rotator(), Location, Rotator);
		LHIKTransform.SetLocation(Location);
		LHIKTransform.SetRotation(Rotator.Quaternion());
	}
	return LHIKTransform;
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
	if (CurrentWeapon) 
	{
		CurrentWeapon->UseStart();
	}
}

void ASBPlayer::UseWeaponOngoing()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->UseOngoing();
	}
}

void ASBPlayer::UseWeaponCompleted()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->UseStop();
	}
}

void ASBPlayer::UseWeaponSpecific1()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SpecificUse1();
	}
}

void ASBPlayer::UseWeaponSpecific2()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SpecificUse2();
	}
}

void ASBPlayer::Zoom()
{
	if (EquippedWeaponType == EEquippedWeapon::EEW_Unarmed)
	{
		ZoomOut();
	}
	else
	{
		if (ZoomState == ECharacterZoomState::ECZS_Zooming)
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
	
}

void ASBPlayer::ChangeWeapon1()
{
	EquipWeapon();
}

void ASBPlayer::ChangeWeapon2()
{
	EquipWeapon();
}

void ASBPlayer::Unarm()
{
	EquippedWeaponType = EEquippedWeapon::EEW_Unarmed;
	CurrentWeapon = nullptr;
	ZoomOut();
}

void ASBPlayer::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void ASBPlayer::EquipWeapon()
{
	EquippedWeaponType = EEquippedWeapon::EEW_Rifle;

	UWorld* World = GetWorld();
	if (CurrentWeapon == nullptr) 
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Owner = this;
		CurrentWeapon = World->SpawnActor<AWeapon>(RifleClass, ActorSpawnParameters);
	}

	if (CurrentWeapon)
	{
		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->GetSkeletalMeshComponent()->AttachToComponent(GetMesh(), AttachmentTransformRules, RightHandSocketName);
		PlayMontage(RifleEquipMontage);
	}
}

void ASBPlayer::ZoomIn()
{
	ZoomState = ECharacterZoomState::ECZS_Zooming;
	MovementSpeedScale = WalkScale;
}

void ASBPlayer::ZoomOut()
{
	ZoomState = ECharacterZoomState::ECZS_NoZoom;
	MovementSpeedScale = JogScale;
}
