#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingCreater.generated.h"

class ABuilding;

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
	void SetPreviewBuilding(uint32 i);
	void DestroyPreviewBuilding();
	void SnapLocation(FVector WorldLocation);

	UFUNCTION(BlueprintCallable)
	void SetGridVisibility(bool bVisibility, bool bForce = false);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuildingCreater)
	TArray<TSubclassOf<ABuilding>> BuildingClasses;

	TArray<TArray<UDecalComponent*>> DecalComponents;

private	:
	ABuilding* PreviewBuilding;
	bool bGridVisibility = true;
};
