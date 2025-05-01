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
	void SetPreviewBuilding(TSubclassOf<ABuilding> BuildingClass);
	void CreateBuilding();
	void DestroyPreviewBuilding();
	void HiddenInGame(bool bNew);
	void SnapLocation(FVector WorldLocation);
	void CancelPreview();
	void RotatePreviewBuildingClockwise90();
	FVector GetCellLocation(int i, int j);

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
	UMaterialInterface* ValidPreviewMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	UMaterialInterface* InvalidPreviewMaterial;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	int CellExtentX = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BuildingCreater)
	int CellExtentY = 10;

	TArray<TArray<UDecalComponent*>> DecalComponents;

private	:
	void UpdateValidCells();
	void UpdateBuildableState();
	void SetPreviewBuildingMaterial(UMaterialInterface* Material);
	void UpdateBuildableStateAndSetPreviewBuildingMaterial();

	TSubclassOf<ABuilding> PreviewBuildingClass;
	TArray<TArray<bool>> bValidCell;
	ABuilding* PreviewBuilding;
	bool bGridVisibility = true;
	bool bBuildable;
};
