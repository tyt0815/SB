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
	UStaticMeshComponent* Grid = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GridBoundary = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building | Grid")
	FIntVector GridExtent = FIntVector(1, 1, 1);
};
