// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridBuilding.h"
#include "PowerFacility.generated.h"



UCLASS()
class SB_API APowerFacility : public AGridBuilding
{
	GENERATED_BODY()
public:
	APowerFacility();

public:
	void ConnectToBuilding(ABuilding* Building);
	void DisconnectToBuilding(ABuilding* Building);
	virtual void PropagatePowerState() override;
	
protected:
	virtual void OnBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	
};
