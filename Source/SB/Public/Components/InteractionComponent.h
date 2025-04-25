#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SB_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
#define AddInteraction(Description, UserObject, FuncName) GetInteractionDelegate(Description)->AddDynamic(UserObject, FuncName);

protected:

private:
	FInteractionDelegate* GetInteractionDelegate(FString Description);

	TMap<FString, int> InteractionDelegateIndices;
	TArray<FInteractionDelegate> InteractionDelegates;
public:
	FORCEINLINE void BroadcastInteraction(FString Description)
	{
		GetInteractionDelegate(Description)->Broadcast();
	}
};
