#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/FacilityPort.h"
#include "OutputPort.generated.h"

UCLASS()
class SB_API AOutputPort : public AFacilityPort
{
	GENERATED_BODY()

public:
	virtual void TryReceivePackage() override;

protected:
	virtual void AddInteractions() override;

	UPROPERTY(BlueprintReadOnly, Category = "Item | Building | ConveyorBelt | FacilityPort | OutputPort")
	bool bInteractive = false;

	int32 LinkedItemDataIndex;

private:
	UFUNCTION()
	void ShowInfo(AActor* Actor);

public:
	FORCEINLINE void SetLinkedItemDataIndex(int32 Index)
	{
		LinkedItemDataIndex;
	}
};
