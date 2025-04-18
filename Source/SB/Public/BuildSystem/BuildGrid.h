#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildGrid.generated.h"

UCLASS()
class SB_API ABuildGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildGrid();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	


protected:
	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* BorderMesh;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* GridMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BuildGrid")
	int GridExtentX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BuildGrid")
	int GridExtentY = 1;
};
