// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ClickableComponent.h"

UClickableComponent::UClickableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UClickableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UClickableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnClicked.AddDynamic(this, &UClickableComponent::OnClicked);
		Owner->OnBeginCursorOver.AddDynamic(this, &UClickableComponent::OnBeginCursorOver);
		Owner->OnEndCursorOver.AddDynamic(this, &UClickableComponent::OnEndCursorOver);
		OwnerMesh = Owner->GetComponentByClass<UStaticMeshComponent>();
	}
}