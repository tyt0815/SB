#include "BuildSystem/BuildCameraPawn.h"
#include "BuildSystem/Building.h"
#include "BuildSystem/BuildingCreater.h"
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

	TraceUnderMouseCursor();

	SetBuildingCreaterLocation();

	switch (BuildMode)
	{
	case EBuildMode::EBM_Interaction:
	{
		ChangeMouseHoveredBuilding();
		break;
	}

	default:
	{
		break;
	};
	}
}

void ABuildCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ABuildCameraPawn::Move);
		EnhancedInputComponent->BindAction(CapsLockInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::CapsLockStarted);
		EnhancedInputComponent->BindAction(XInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::XStarted);
		EnhancedInputComponent->BindAction(RInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::RStarted);
		EnhancedInputComponent->BindAction(MouseLInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::MouseLStarted);
		EnhancedInputComponent->BindAction(MouseRInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::MouseRStarted);
		if (NumberInputActions.IsValidIndex(1) && NumberInputActions[1])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[1], ETriggerEvent::Started, this, &ABuildCameraPawn::Num1Started);
		}
		if (NumberInputActions.IsValidIndex(2) && NumberInputActions[2])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[2], ETriggerEvent::Started, this, &ABuildCameraPawn::Num2Started);
		}
		if (NumberInputActions.IsValidIndex(3) && NumberInputActions[3])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[3], ETriggerEvent::Started, this, &ABuildCameraPawn::Num3Started);
		}
		if (NumberInputActions.IsValidIndex(4) && NumberInputActions[4])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[4], ETriggerEvent::Started, this, &ABuildCameraPawn::Num4Started);
		}
		if (NumberInputActions.IsValidIndex(5) && NumberInputActions[5])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[5], ETriggerEvent::Started, this, &ABuildCameraPawn::Num5Started);
		}
		if (NumberInputActions.IsValidIndex(6) && NumberInputActions[6])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[6], ETriggerEvent::Started, this, &ABuildCameraPawn::Num6Started);
		}
		if (NumberInputActions.IsValidIndex(7) && NumberInputActions[7])
		{
			EnhancedInputComponent->BindAction(NumberInputActions[7], ETriggerEvent::Started, this, &ABuildCameraPawn::Num7Started);
		}

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

	// Spawn BuildingCreater
	if (BuildingCreaterClass)
	{
		BuildingCreater = GetWorld()->SpawnActor<ABuildingCreater>(BuildingCreaterClass);
	}
	else
	{
		BuildingCreater = GetWorld()->SpawnActor<ABuildingCreater>();
		SCREEN_LOG_NONE_KEY(TEXT("No SubClass of ABuildingCreater"));
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

void ABuildCameraPawn::CapsLockStarted()
{
	if (PlayerCharacter)
	{
		TransferPlayerControllerPossessionToPawn(PlayerCharacter);
		PlayerCharacter->OnPlayerPossessStarted();
	}
}

void ABuildCameraPawn::XStarted()
{
	if (IsValid(SelectedBuilding))
	{
		SelectedBuilding->Destroy();
		SelectedBuilding = nullptr;
	}
}

void ABuildCameraPawn::RStarted()
{
	if (BuildingCreater)
	{
		BuildingCreater->RotatePreviewBuildingClockwise90();
	}
}

void ABuildCameraPawn::MouseLStarted()
{
	switch (BuildMode)
	{
	case EBuildMode::EBM_Interaction:
	{
		ChangeSelectedBuilding(MouseHoveredBuilding);
		break;
	}
	case EBuildMode::EBM_Placement:
	{
		BuildingCreater->CreateBuilding();
		EndPlacementMode();
	}
	}
	
}

void ABuildCameraPawn::MouseRStarted()
{
	switch (BuildMode)
	{
	case EBuildMode::EBM_Interaction:
	{
		break;
	}
	case EBuildMode::EBM_Placement:
	{
		EndPlacementMode();
	}
	default:
		break;
	}
}

void ABuildCameraPawn::EndPlacementMode()
{
	BuildingCreater->DestroyPreviewBuilding();
	BuildingCreater->HiddenInGame(true);
	BuildMode = EBuildMode::EBM_Interaction;
}

void ABuildCameraPawn::Num1Started()
{
	NumberStarted(1);
}

void ABuildCameraPawn::Num2Started()
{
	NumberStarted(2);
}

void ABuildCameraPawn::Num3Started()
{
	NumberStarted(3);
}

void ABuildCameraPawn::Num4Started()
{
	NumberStarted(4);
}

void ABuildCameraPawn::Num5Started()
{
	NumberStarted(5);
}

void ABuildCameraPawn::Num6Started()
{
	NumberStarted(6);
}

void ABuildCameraPawn::Num7Started()
{
	NumberStarted(7);
}

void ABuildCameraPawn::SelectBuilding(ABuilding* Building)
{
	DeselectBuilding();

	SelectedBuilding = Building;
	if (SelectedBuilding)
	{
		SelectedBuilding->OnSelected();
	}
}

void ABuildCameraPawn::DeselectBuilding()
{
	if (SelectedBuilding)
	{
		SelectedBuilding->OnDeselected();
	}
	SelectedBuilding = nullptr;
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

void ABuildCameraPawn::TraceUnderMouseCursor()
{
	FVector MouseWorldPosition;
	FVector MouseWorldDirection;
	GetMouseWorldPosition(MouseWorldPosition, MouseWorldDirection);

	FVector LineTraceEnd = MouseWorldPosition + MouseWorldDirection * 10000.0f;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(PlayerCharacter);
	ActorsToIgnore.Add(BuildingCreater);
	UKismetSystemLibrary::LineTraceSingle(
		this,
		MouseWorldPosition,
		LineTraceEnd,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		MouseDownTraceHit,
		true
	);
}

FVector ABuildCameraPawn::CalculatePlacementLocation(ABuilding* Building)
{
	FVector Location = MouseDownTraceHit.ImpactPoint;
	Location.Z += Building->GetZOffset();
	return Location;
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

void ABuildCameraPawn::ChangeMouseHoveredBuilding()
{
	ABuilding* Building = Cast<ABuilding>(MouseDownTraceHit.GetActor());
	if (MouseHoveredBuilding == Building)
	{
		
	}
	else 
	{
		if (MouseHoveredBuilding)
		{
			MouseHoveredBuilding->OnMouseHoverEnded();
		}

		MouseHoveredBuilding = Building;
		if (MouseHoveredBuilding)
		{
			Building->OnMouseHoverStarted();
		}
	}
}

void ABuildCameraPawn::ChangeSelectedBuilding(ABuilding* Building)
{
	if (SelectedBuilding == Building)
	{
		DeselectBuilding();
	}
	else
	{
		SelectBuilding(MouseHoveredBuilding);
	}
}

void ABuildCameraPawn::NumberStarted(int i)
{
	switch (BuildMode)
	{
	case EBuildMode::EBM_Interaction:
	{
		SwitchToPlacementMode(i);
	}
	}
}

void ABuildCameraPawn::SwitchToPlacementMode(int i)
{
	BuildMode = EBuildMode::EBM_Placement;
	if (PlayerCharacter)
	{
		TArray<TSubclassOf<ABuilding>> BuildingList = PlayerCharacter->GetBuildingList();
		if (BuildingList.IsValidIndex(i))
		{
			BuildingCreater->SetPreviewBuilding(BuildingList[i]);
		}
	}
	
}

void ABuildCameraPawn::SetBuildingCreaterLocation()
{
	BuildingCreater->SnapLocation(MouseDownTraceHit.ImpactPoint);
}
