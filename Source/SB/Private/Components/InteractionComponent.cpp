#include "Components/InteractionComponent.h"
#include "SB/DebugMacro.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

int32 UInteractionComponent::AddInteraction(FName Description)
{
	int Index = Interactions.Num();
	Interactions.Add(FInteractionInfo());
	Interactions[Index].Description = Description;
	return Index;
}