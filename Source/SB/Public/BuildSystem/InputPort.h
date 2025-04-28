// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/FacilityPort.h"
#include "Items/PackageReceiver.h"
#include "InputPort.generated.h"

UCLASS()
class SB_API AInputPort : public AFacilityPort, public IPackageReceiver
{
	GENERATED_BODY()
	
public:	
	AInputPort();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual bool ReceivePackage_Implementation(APackagedItem* Package) override;
};
