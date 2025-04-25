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

FInteractionDelegate* UInteractionComponent::GetInteractionDelegate(FString Description)
{
	if (!InteractionDelegateIndices.Find(Description))
	{
		InteractionDelegateIndices.Add(Description, InteractionDelegates.Num());
		InteractionDelegates.Add(FInteractionDelegate());
	}
	int i = InteractionDelegateIndices[Description];
	return &InteractionDelegates[i];
}