// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/FacilityPort.h"
#include "InputPort.generated.h"

UCLASS()
class SB_API AInputPort : public AFacilityPort
{
	GENERATED_BODY()
	
public:	
	AInputPort();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TrySupplyPackage() override;
};
