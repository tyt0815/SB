#include "Characters/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ�ѷ��� ȸ���� ĳ���Ͱ� ȸ������ �ʵ��� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
	GetCharacterMovement()->bOrientRotationToMovement = true;	// ĳ���Ͱ� ������ �������� ȸ���ϵ��� ����
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);	// ȸ�� �ӵ�

	// ������ �� ������Ʈ ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);	
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true; // ī�޶� ��Ʈ�ѷ��� ���� ȸ���ǵ���	����

	// ī�޶� ������Ʈ ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	// ī�޶� ������ �Ͽ� �����ǵ��� ����. USpringArmComponent::SocketName�� "SpringEndPoint"�� ������ �� ���κп� �����ǵ��� ����
	FollowCamera->bUsePawnControlRotation = false;	// ī�޶� ��Ʈ�ѷ��� ���� ȸ������ �ʵ��� ����

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
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
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
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
