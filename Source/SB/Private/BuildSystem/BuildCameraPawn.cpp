#include "BuildSystem/BuildCameraPawn.h"
#include "BuildSystem/Building.h"
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

	switch (BuildMode)
	{
	case EBuildMode::EBM_Interaction:
	{
		ChangeMouseHoveredBuilding();
		break;
	}

	case EBuildMode::EBM_Placement:
	{
		check(GetCurrentPreviewBuilding());

		GetCurrentPreviewBuilding()->SetActorLocation(CalculatePlacementLocation(GetCurrentPreviewBuilding()));
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
		EnhancedInputComponent->BindAction(ToggleToPlayerCharacterInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::ToggleToPlayerCharacter);		EnhancedInputComponent->BindAction(ToggleToPlayerCharacterInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::ToggleToPlayerCharacter);			EnhancedInputComponent->BindAction(ToggleToPlayerCharacterInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::ToggleToPlayerCharacter);		EnhancedInputComponent->BindAction(ToggleToPlayerCharacterInputAction, ETriggerEvent::Started, this, &ABuildCameraPawn::ToggleToPlayerCharacter);
		EnhancedInputComponent->BindAction(MouseLInputAction, ETriggerEvent::Triggered, this, &ABuildCameraPawn::MouseLTriggered);
		EnhancedInputComponent->BindAction(MouseRInputAction, ETriggerEvent::Triggered, this, &ABuildCameraPawn::MouseRTriggered);
		EnhancedInputComponent->BindAction(Num1InputAction, ETriggerEvent::Triggered, this, &ABuildCameraPawn::Num1Triggerd);
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

	// CreatePreviewBuildings
	{
		PreviewBuildings.SetNum(PreviewBuildingClasses.Num());
		for (int i = 0; i < PreviewBuildings.Num(); ++i)
		{
			if (PreviewBuildingClasses[i])
			{
				PreviewBuildings[i] = GetWorld()->SpawnActor<ABuilding>(PreviewBuildingClasses[i]);
				if (PreviewBuildings[i])
				{
					PreviewBuildings[i]->SetVisibility(false);
					// PreviewBuildings[i]->SetAsPreview();
				}
			}
		}
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

void ABuildCameraPawn::MouseLTriggered()
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
		check(GetCurrentPreviewBuilding());
		if (PreviewBuildingClasses[CurrentPreviewBuildingIndex] && MouseDownTraceHit.GetActor())
		{
			FActorSpawnParameters SpawnParam;
			ABuilding* Building = GetWorld()->SpawnActor<ABuilding>(PreviewBuildingClasses[CurrentPreviewBuildingIndex]);
			if (Building)
			{
				Building->SetActorLocation(CalculatePlacementLocation(Building));
			}
		}
		EndPlacementMode();
	}
	}
	
}

void ABuildCameraPawn::MouseRTriggered()
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
	CurrentPreviewBuildingIndex = 0;
	if (GetCurrentPreviewBuilding())
	{
		GetCurrentPreviewBuilding()->SetVisibility(false);
	}
	BuildMode = EBuildMode::EBM_Interaction;
}

void ABuildCameraPawn::Num1Triggerd()
{
	switch (BuildMode)
	{
	case EBuildMode::EBM_Interaction:
	{
		SwitchToPlacementMode(1);
	}
	}
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
	ActorsToIgnore.Add(GetCurrentPreviewBuilding());
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

void ABuildCameraPawn::ChangePreviewBuilding(int i)
{
	ABuilding* OldBuilding = GetCurrentPreviewBuilding();
	ABuilding* NewBuilding = nullptr;
	if (PreviewBuildings.IsValidIndex(i))
	{
		NewBuilding = PreviewBuildings[i];
	}
	if (NewBuilding == OldBuilding)
	{

	}
	else
	{
		if (OldBuilding)
		{
			OldBuilding->SetVisibility(false);
		}
		if (NewBuilding)
		{
			NewBuilding->SetVisibility(true);
		}
		CurrentPreviewBuildingIndex = i;
	}
}

void ABuildCameraPawn::SwitchToPlacementMode(int PreviewBuildingIndex)
{
	if (PreviewBuildings.IsValidIndex(PreviewBuildingIndex) && PreviewBuildings[PreviewBuildingIndex])
	{
		BuildMode = EBuildMode::EBM_Placement;
		ChangePreviewBuilding(PreviewBuildingIndex);
	}
}