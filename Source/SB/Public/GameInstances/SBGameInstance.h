#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Items/PackagedItem.h"
#include "SBGameInstance.generated.h"

class APackagedItem;

UCLASS()
class SB_API USBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SBGameInstance")
	TSubclassOf<APackagedItem> PackagedItemClass;

public:
	FORCEINLINE UClass* GetPackagedItemClass() const
	{
		return PackagedItemClass;
	}
};
