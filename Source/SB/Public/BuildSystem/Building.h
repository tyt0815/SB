// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class UBoxComponent;

UCLASS()
class SB_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuilding();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void OnConstruction(const FTransform& Transform) override;
	void OnMouseHoverStarted();
	void OnMouseHoverEnded();
	void OnSelected();
	void OnDeselected();
	float GetZOffset() const;
	float GetHalfWidth() const;
	float GetHalfDepth() const;
	float GetHalfHeight() const;
	float GetWidth() const;
	float GetDepth() const;
	float GetHeight() const;
	void SetVisibility(bool bVisibility);
	void SetAsPreview();
	void SetAllMaterials(UMaterialInterface* Material);

	UFUNCTION(BlueprintCallable, Category = "Building")
	void SnapLocation(FVector WorldLocation);


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BuildBlocker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	FIntVector CellExtent = FIntVector(1, 1, 1);

	UPROPERTY(BlueprintReadOnly, Category = Building)
	bool bSelected = false;

private:
	// 외곽선을 그린다. 색상은 PP_HightlightMaterial_Inst 의 색상을 참고.
	void SetOutlineDraw(bool bDraw, int Color);
	void SetRenderCustomDepthStencil(bool bRender, int Stencil);

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const
	{
		return Mesh;
	}
	FORCEINLINE UBoxComponent* GetBuildBlocker() const
	{
		return BuildBlocker;
	}
	FORCEINLINE FIntVector GetCellExtent() const
	{
		return CellExtent;
	}
};
