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

public:
	void AddProductionInput(FItemData ItemData);
	void StartProduction();
	FItemData CollectOutput();


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInventoryComponent* ProductionInputs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInventoryComponent* ProductionOutputs;

private:
	void TryAddToProductionOutput();

	UPROPERTY(EditAnywhere, Category = "Item | Building | ProductionFacility")
	int32 InputPortNum = 3;

	UPROPERTY(EditAnywhere, Category = "Item | Building | ProductionFacility")
	int32 OutputPortNum = 3;

	UPROPERTY(EditAnywhere, Category = "Item | Building | ProductionFacility")
	TArray<FProductionRecipe> Recipes;

	FItemData OutputData;
	float ProductionTimeLeft = 0;
	bool bProducted = false;
};
