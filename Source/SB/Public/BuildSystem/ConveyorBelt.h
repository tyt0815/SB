// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/Building.h"
#include "Items/PackageReceiver.h"
#include "Items/PackageSupplier.h"
#include "ConveyorBelt.generated.h"

class USplineComponent;
class APackagedItem;

enum class EConveyorBeltState : uint8
{
	ECBS_Empty,
	ECBS_Moving,
	ECBS_Stop
};
 
UCLASS()
class SB_API AConveyorBelt : public ABuilding, public IPackageReceiver, public IPackageSupplier
{
	GENERATED_BODY()

public:
	AConveyorBelt();
	virtual void Tick(float Delta) override;

	void TrySupplyPackage();

	void TryReceivePackage();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ReceivePackage_Implementation(APackagedItem* Package) override;
	virtual APackagedItem* SupplyPackage_Implementation() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USplineComponent* SplineComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Building | ConveyorBelt")
	APackagedItem* CarriedPackage;

	UPROPERTY(BlueprintReadOnly, Category = "Item | Building | ConveyorBelt")
	float MoveDuration = 1.0f;
	
	float ElapsedMoveTime = 0.0f;

private:
	AConveyorBelt* TraceNextConveyorBelt();
	AConveyorBelt* TracePrevConveyorBelt();
	AActor* TraceReceiver();
	AActor* TraceSupplier();

	AConveyorBelt* NextConveyorBelt;
	AConveyorBelt* PrevConveyorBelt;
};
