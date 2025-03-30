// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SBPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASBPlayer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASBPlayer::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASBPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASBPlayer::Look);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &ASBPlayer::Zoom);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASBPlayer::Fire);
		EnhancedInputComponent->BindAction(ChangeWeapon1Action, ETriggerEvent::Started, this, &ASBPlayer::ChangeWeapon1);
		EnhancedInputComponent->BindAction(ChangeWeapon2Action, ETriggerEvent::Started, this, &ASBPlayer::ChangeWeapon2);
		EnhancedInputComponent->BindAction(UnarmAction, ETriggerEvent::Started, this, &ASBPlayer::Unarm);
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

void ASBPlayer::Fire(const FInputActionValue& Value)
{
	if (ZoomState != ECharacterZoomState::ECZS_Zooming) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (EquippedWeaponType == EEquippedWeapon::EEW_Rifle && RifleFireMontage)
		{
			AnimInstance->Montage_Play(RifleFireMontage);
		}
	}
}

void ASBPlayer::Zoom(const FInputActionValue& Value)
{
	if (EquippedWeaponType == EEquippedWeapon::EEW_Unarmed)
	{
		ZoomState = ECharacterZoomState::ECZS_NoZoom;
		MovementSpeedScale = JogScale;
	}
	else
	{
		if (ZoomState == ECharacterZoomState::ECZS_Zooming)
		{
			ZoomState = ECharacterZoomState::ECZS_NoZoom;
			MovementSpeedScale = JogScale;
		}
		else if (ZoomState == ECharacterZoomState::ECZS_NoZoom)
		{
			ZoomState = ECharacterZoomState::ECZS_Zooming;
			MovementSpeedScale = WalkScale;
		}
		else
		{
			SCREEN_LOG_NONE_KEY("Undefined ZoomState: ASBPlayer::Zoom");
		}
	}
}

void ASBPlayer::ChangeWeapon1(const FInputActionValue& Value)
{
	SCREEN_LOG(0, "ChangeWeapon1");
	EquippedWeaponType = EEquippedWeapon::EEW_Rifle;
}

void ASBPlayer::ChangeWeapon2(const FInputActionValue& Value)
{
	SCREEN_LOG(0, "ChangeWeapon2");
	EquippedWeaponType = EEquippedWeapon::EEW_Rifle;
}

void ASBPlayer::Unarm(const FInputActionValue& Value)
{
	SCREEN_LOG(0, "Unarm");
	EquippedWeaponType = EEquippedWeapon::EEW_Unarmed;
}
