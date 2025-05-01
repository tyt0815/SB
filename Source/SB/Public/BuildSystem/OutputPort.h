#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/FacilityPort.h"
#include "OutputPort.generated.h"



UCLASS()
class SB_API AOutputPort : public AFacilityPort
{
	GENERATED_BODY()
public:
	virtual void Tick(float Delta) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TryReceivePackage() override;
	FItemData* GetLinkedItemData();
	void SetLinkedItemClass(TSubclassOf<AItem> ItemClass);

protected:
	virtual void AddInteractions() override;
	virtual void TraceSupplier() override {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Building | ConveyorBelt | FacilityPort | OutputPort")
	bool bInteractive = false;

	TSubclassOf<AItem> LinkedItemClass;

private:
	UFUNCTION()
	void ShowInfo(AActor* Actor);

public:
};
