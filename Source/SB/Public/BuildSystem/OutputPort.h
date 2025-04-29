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
	int32 LinkedItemDataIndex;

public:
	FORCEINLINE void SetLinkedItemDataIndex(int32 Index)
	{
		LinkedItemDataIndex;
	}
};
