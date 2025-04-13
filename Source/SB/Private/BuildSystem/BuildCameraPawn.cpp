#include "BuildSystem/BuildCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SB/DebugMacro.h"
#include "Characters/Player/SBPlayer.h"
#include "PlayerController/SBPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

ABuildCameraPawn::ABuildCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	SetRootComponent(FollowCamera);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

void ABuildCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ABuildCameraPawn::Move);
		EnhancedInputComponent->BindAction(ToggleToPlayerCharacterInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::ToggleToPlayerCharacter);		EnhancedInputComponent->BindAction(ToggleToPlayerCharacterInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::ToggleToPlayerCharacter);			EnhancedInputComponent->BindAction(ToggleToPlayerCharacterInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::ToggleToPlayerCharacter);		EnhancedInputComponent->BindAction(ToggleToPlayerCharacterInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::ToggleToPlayerCharacter);
		EnhancedInputComponent->BindAction(Mouse1InputAction, ETriggerEvent::Triggered, this, &ABuildCameraPawn::Mouse1Triggered);
	}
}

void ABuildCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ASBPlayer>(GetOwner());
	if (PlayerCharacter == nullptr)
	{
		SCREEN_LOG_NONE_KEY("ABuildCameraPawn class must owned by ASBPlayer when BeginPlay");
	}
}

void ABuildCameraPawn::OnPlayerPossessStarted_Implementation()
{
	UCameraComponent* PlayerCamera = PlayerCharacter->GetCameraComponent();
	if (PlayerCamera)
	{
		SetActorLocation(PlayerCamera->GetComponentLocation());
		SetActorRotation(PlayerCamera->GetComponentRotation());
		FollowCamera->FieldOfView = PlayerCamera->FieldOfView;
	}
	InitializePlayerController();
}

void ABuildCameraPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		FVector Direction(MovementVector.X, MovementVector.Y, 0.0f);
		AddMovementInput(Direction);
	}
}

void ABuildCameraPawn::ToggleToPlayerCharacter()
{
	if (PlayerCharacter)
	{
		TransferPlayerControllerPossessionToPawn(PlayerCharacter);
		PlayerCharacter->OnPlayerPossessStarted();
	}
}

void ABuildCameraPawn::Mouse1Triggered(const FInputActionValue& Value)
{
	FVector MouseWorldPosition;
	FVector MouseWorldDirection;
	GetMouseWorldPosition(MouseWorldPosition, MouseWorldDirection);

	FVector LineTraceEnd = MouseWorldPosition + MouseWorldDirection * 10000.0f;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(
		this,
		MouseWorldPosition,
		LineTraceEnd,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);
	if (HitResult.GetActor())
	{
		SCREEN_LOG_NONE_KEY(HitResult.GetActor()->GetName());
	}
}

void ABuildCameraPawn::SetInputMappingContext()
{
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

void ABuildCameraPawn::InitializePlayerController()
{
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->AddInputMappingContext(DefaultMappingContext);
		PlayerController->SetMouseInterface(true);
	}
}

void ABuildCameraPawn::TransferPlayerControllerPossessionToPawn(APawn* Pawn)
{
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->RemoveInputMappingContext(DefaultMappingContext);
		PlayerController->UnPossess();
		PlayerController->Possess(Pawn);
	}
}

void ABuildCameraPawn::GetMouseWorldPosition(FVector& WorldLocation, FVector& WorldDirection)
{
	ASBPlayerController* PlayerController = Cast<ASBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	}
}
