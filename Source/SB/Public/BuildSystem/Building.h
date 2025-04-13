// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class UBoxComponent;

UCLASS()
class SB_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuilding();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
};
