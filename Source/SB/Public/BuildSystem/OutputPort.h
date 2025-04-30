#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/FacilityPort.h"
#include "OutputPort.generated.h"

UCLASS()
class SB_API AOutputPort : public AFacilityPort
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void TryReceivePackage() override;

protected:
	virtual void AddInteractions() override;
	virtual void TraceSupplier() override {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Building | ConveyorBelt | FacilityPort | OutputPort")
	bool bInteractive = false;

	int32 LinkedItemDataIndex = -1;

private:
	UFUNCTION()
	void ShowInfo(AActor* Actor);

public:
	FORCEINLINE void SetLinkedItemDataIndex(int32 Index)
	{
		LinkedItemDataIndex;
	}
};
