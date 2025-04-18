// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SB_API UBuildSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuildSystemComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:	

protected:
	UPROPERTY(EditAnywhere, Category = BuildSystem)
	int CellSize = 50;

	UPROPERTY(EditAnywhere, Category = BuildSystem)
	int CellExtentX = 10;

	UPROPERTY(EditAnywhere, Category = BuildSystem)
	int CellExtentY = 10;
};
