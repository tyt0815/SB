// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/Building.h"
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
class SB_API AConveyorBelt : public ABuilding
{
	GENERATED_BODY()

public:
	AConveyorBelt();
	virtual void Tick(float Delta) override;

	void CarryPackage(float Delta);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TrySupplyPackage();
	virtual void TryReceivePackage();
	virtual bool ReceivePackage(APackagedItem* Package);
	virtual APackagedItem* SupplyPackage();
	
protected:
	virtual void TraceReceiver();
	virtual void TraceSupplier();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USplineComponent* SplineComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Building | ConveyorBelt")
	APackagedItem* CarriedPackage;

	UPROPERTY(BlueprintReadOnly, Category = "Item | Building | ConveyorBelt")
	float MoveDuration = 1.0f;
	
	float ElapsedMoveTime = 0.0f;

private:

	AConveyorBelt* Receiver;
	AConveyorBelt* Supplier;
};
