#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/FacilityPort.h"
#include "Items/PackageSupplier.h"
#include "OutputPort.generated.h"

UCLASS()
class SB_API AOutputPort : public AFacilityPort, public IPackageSupplier
{
	GENERATED_BODY()
	
public:	
	AOutputPort();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual APackagedItem* SupplyPackage_Implementation() override;

protected:
	int32 LinkedItemDataIndex;

public:
	FORCEINLINE void SetLinkedItemDataIndex(int32 Index)
	{
		LinkedItemDataIndex;
	}
};
