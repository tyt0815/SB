// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Building.generated.h"

class UBoxComponent;
class ACentralHubBuilding;
class APowerFacility;

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	EBT_StandAlone UMETA(DisplayName = "StandAlone"),
	EBT_HubLinkedFacility UMETA(DisplayName = "HubLinkedFacility"),
	EBT_PowerLinkedFacility UMETA(DisplayName = "PowerLinkedFacility")
};

UCLASS()
class SB_API ABuilding : public AItem
{
	GENERATED_BODY()
	
public:	
	ABuilding();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	void IncreasePowerConsumption(uint16 Power);
	void DecreasePowerConsumption(uint16 Power);
	virtual void PropagatePowerState();
	virtual void Place(FVector WorldLocation, FRotator WorldRotation);
	virtual void TryConnectToNearByFacility();
	void TraceGridBuilding(TArray<FHitResult>& HitResults);
	virtual void OnConstruction(const FTransform& Transform) override;
	void OnMouseHoverStarted();
	void OnMouseHoverEnded();
	void OnSelected();
	void OnDeselected();
	virtual void OnConnectToBuilding(ABuilding* Parent);
	virtual void OnDisconnectToBuilding();
	void TryConnectToBuilding(ABuilding* Parent);
	void TryDisconnectToBuilding();
	float GetZOffset() const;
	float GetHalfWidth() const;
	float GetHalfDepth() const;
	float GetHalfHeight() const;
	float GetWidth() const;
	float GetDepth() const;
	float GetHeight() const;
	void SetVisibility(bool bVisibility);
	virtual void SetAsPreview();
	virtual void SetAllMaterials(UMaterialInterface* Material);
	void SetPowerState(bool bOn);
	// 외곽선을 그린다. 색상은 PP_HightlightMaterial_Inst 의 색상을 참고.
	void SetOutlineDraw(bool bDraw, int Color);
	virtual bool IsOperating() const;
	bool IsPowerOn() const;
	bool HasSufficientPower() const;
	void RotateClockwise90();

	UFUNCTION(BlueprintCallable, Category = "Building")
	void SnapLocationXY(FVector WorldLocation);

protected:
	virtual void InitMeshsVisibilityAndPhysics() override;
	virtual void BeginDestroy() override;
	virtual void OnPickedUp(AActor* OtherActor) override;
	void TraceBuilding(FVector Start, FVector End, FHitResult& HitResult);
	void TraceBuildings(FVector Start, FVector End, TArray<FHitResult>& HitResults);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BuildBlocker;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Building")
	FIntVector CellExtent = FIntVector(1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Building")
	EBuildingType BuildingType = EBuildingType::EBT_StandAlone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Building")
	int PowerConsumption = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Item | Building")
	bool bSelected = false;

	ABuilding* ParentBuilding;

	bool bPowerOn = true;
private:
	
	void SetRenderCustomDepthStencil(bool bRender, int Stencil);

	bool bPreview = false;

public:
	FORCEINLINE UBoxComponent* GetBuildBlocker() const
	{
		return BuildBlocker;
	}
	FORCEINLINE FIntVector GetCellExtent() const
	{
		return CellExtent;
	}
	FORCEINLINE EBuildingType GetBuildingType() const
	{
		return BuildingType;
	}
	FORCEINLINE bool IsPreview() const
	{
		return bPreview;
	}
	FORCEINLINE bool IsConnectedToParentBuilding() const
	{
		return IsValid(ParentBuilding);
	}
};
