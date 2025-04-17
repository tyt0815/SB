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
	void OnMouseHoverStarted();
	void OnMouseHoverEnded();
	void OnSelected();
	void OnDeselected();
	float GetZOffset() const;
	void SetVisibility(bool bVisibility);
	void SetAsPreview();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, Category = "States")
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
	FORCEINLINE UBoxComponent* GetBoxComponent() const
	{
		return BoxComponent;
	}
};
