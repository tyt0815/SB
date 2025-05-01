#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/Building.h"
#include "ProductionFacility.generated.h"

class UInventoryComponent;

USTRUCT(BlueprintType)
struct FProductionRecipe
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AItem>, int32> Ingrediant;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> OutputItemClass;

	UPROPERTY(EditAnywhere)
	float ProductionTime = 2;
};


UCLASS()
class SB_API AProductionFacility : public ABuilding
{
	GENERATED_BODY()
	
public:
	AProductionFacility();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float Delta) override;

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	void StartProduction();
	FItemData* GetRemainedItemData();

protected:
	virtual void AddInteractions() override;

	UFUNCTION()
	void ShowInfoWidget(AActor* OtherActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInventoryComponent* ProductionInputs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInventoryComponent* ProductionOutputs;

private:
	void TryAddToProductionOutput();

	UPROPERTY(EditAnywhere, Category = "Item | Building | ProductionFacility")
	TSubclassOf<class AInputPort> InputPortClass;

	UPROPERTY(EditAnywhere, Category = "Item | Building | ProductionFacility")
	TSubclassOf<class AOutputPort> OutputPortClass;

	UPROPERTY(EditAnywhere, Category = "Item | Building | ProductionFacility")
	TArray<FIntVector> InputPortGridCoord;

	UPROPERTY(EditAnywhere, Category = "Item | Building | ProductionFacility")
	TArray<FIntVector> OutputPortGridCoord;

	UPROPERTY(EditAnywhere, Category = "Item | Building | ProductionFacility")
	TArray<FProductionRecipe> Recipes;

	TArray<UChildActorComponent*> InputPortComponents;
	TArray<UChildActorComponent*> OutputPortComponents;
	FItemData OutputData;
	float ProductionTimeLeft = 0;
	bool bProducted = false;

};
