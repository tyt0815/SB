#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/Building.h"
#include "GridBuilding.generated.h"


UCLASS()
class SB_API AGridBuilding : public ABuilding
{
	GENERATED_BODY()
	
public:
	AGridBuilding();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetAsPreview() override;

protected:
	void TraceBuildingsInBoundary(TArray<FHitResult>& HitResults);

	UFUNCTION()
	virtual void OnBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* GridBoundaryBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GridBoundaryMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	FIntVector GridExtent = FIntVector(1, 1, 1);

	UPROPERTY(VisibleAnywhere, Category = "Building")
	TArray<ABuilding*> ChildBuildings;
private:
};
