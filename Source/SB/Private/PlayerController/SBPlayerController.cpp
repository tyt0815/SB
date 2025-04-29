#include "PlayerController/SBPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ASBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(EnhancedInputLocalPlayerSubsystem);
}

void ASBPlayerController::AddInputMappingContext(UInputMappingContext* InputMappingContext)
{
	EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
}

void ASBPlayerController::RemoveInputMappingContext(UInputMappingContext* InputMappingContext)
{
	EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
}

void ASBPlayerController::SetMouseInterface(bool bUse)
{
	bShowMouseCursor = bUse;
	bEnableClickEvents = bUse;
	bEnableMouseOverEvents = bUse;
}

void ASBPlayerController::SwitchToUIMode(bool bUI)
{
	SetMouseInterface(bUI);
	bUseUI = bUI;
}