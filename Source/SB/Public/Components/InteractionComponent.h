#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

#define AddInteractionAt(Index, UserObject, FuncName) GetInteractionDelegate(Index)->AddDynamic(UserObject, FuncName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionDelegate, AActor*, OtherActor);

USTRUCT(BlueprintType)
struct FInteractionInfo
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere)
	FName Description = "Undefined";

	FInteractionDelegate InteractionDelegate;
};

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
	int32 AddInteraction(FName Description);

protected:

private:
	TArray<FInteractionInfo> Interactions;

public:
	FORCEINLINE int32 GetInteractionNum() const
	{
		return Interactions.Num();
	}
	FORCEINLINE bool IsValidIndex(int32 i)
	{
		return Interactions.IsValidIndex(i);
	}
	FORCEINLINE FInteractionInfo* GetInteractionInfoAt(int32 i)
	{
		return &Interactions[i];
	}
	FORCEINLINE FName GetInteractionDescriptionAt(int32 i)
	{
		return Interactions[i].Description;
	}
	FORCEINLINE FInteractionDelegate* GetInteractionDelegate(int32 i)
	{
		return &Interactions[i].InteractionDelegate;
	}
	FORCEINLINE void BroadcastInteraction(int32 Index, AActor* OtherActor)
	{
		GetInteractionDelegate(Index)->Broadcast(OtherActor);
	}
};
