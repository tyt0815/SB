// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SBPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class SB_API ASBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;
	
public:
	void AddInputMappingContext(UInputMappingContext* InputMappingContext);
	void RemoveInputMappingContext(UInputMappingContext* InputMappingContext);
	void SetMouseInterface(bool bUse);

protected:

private:
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem;
};
