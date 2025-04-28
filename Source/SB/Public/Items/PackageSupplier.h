// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PackageSupplier.generated.h"

class APackagedItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPackageSupplier : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SB_API IPackageSupplier
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "PackageSupplier")
	APackagedItem* SupplyPackage();

protected:

};
