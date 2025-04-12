// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClickableComponent.generated.h"


UCLASS(Blueprintable,  ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SB_API UClickableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UClickableComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "MouseCallback")
	void OnClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION(BlueprintImplementableEvent, Category = "MouseCallback")
	void OnBeginCursorOver(AActor* TouchedActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "MouseCallback")
	void OnEndCursorOver(AActor* TouchedActor);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "ClickableComponent")
	UStaticMeshComponent* OwnerMesh;

public:
};
