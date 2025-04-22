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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* GridBoundaryBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GridBoundaryMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building | Grid")
	FIntVector GridExtent = FIntVector(1, 1, 1);

private:
	UFUNCTION()
	virtual void OnPlayerBeginOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnPlayerEndOverlapGridBoundary(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
