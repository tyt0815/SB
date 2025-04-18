#include "BuildSystem/BuildSystemComponent.h"
#include "Components/DecalComponent.h"

UBuildSystemComponent::UBuildSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBuildSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBuildSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}