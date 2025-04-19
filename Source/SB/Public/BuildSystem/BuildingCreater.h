#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingCreater.generated.h"

class FMaterial;

UCLASS()
class SB_API ABuildingCreater : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildingCreater();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void SetGridVisibility(bool bVisibility);

	UFUNCTION(BlueprintCallable)
	void SetGridMaterial(UMaterialInterface* Material);

protected:

	UPROPERTY(EditAnywhere, Category = Components)
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	UMaterialInterface* ValidCellMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	UMaterialInterface* InvalidCellMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	UMaterialInterface* PreviewMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	int CellSize = 100;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	int CellExtentX = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	int CellExtentY = 10;

	TArray<TArray<UDecalComponent*>> DecalComponents;
};
