// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UInteractionComponent;

UCLASS()
class SB_API AItem : public AActor
{
	GENERATED_BODY()
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:	


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, Category = Item)
	FName ItemName;

	UPROPERTY(EditAnywhere, Category = Item)
	UTexture2D* Thumnail;

	UPROPERTY(EditAnywhere, Category = Item)
	bool bStackable;

private:
	void ActivateStaticMesh(bool bActive);
	void ActivateSkeletalMesh(bool bActive);

public:
	FORCEINLINE FName GetItemName() const
	{
		return ItemName;
	}
	FORCEINLINE UTexture2D* GetThumnail() const
	{
		return Thumnail;
	}
	FORCEINLINE bool IsStackable() const
	{
		return bStackable;
	}
	FORCEINLINE UStaticMeshComponent* GetStaticMesh() const
	{
		return StaticMesh;
	}
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() const
	{
		return SkeletalMesh;
	}
};
