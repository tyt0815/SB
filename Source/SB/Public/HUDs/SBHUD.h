#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SBHUD.generated.h"

class USBPlayerOverlayWidget;

UCLASS()
class SB_API ASBHUD : public AHUD
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = SB)
	TSubclassOf<USBPlayerOverlayWidget> PlayerOverlayClass;

	UPROPERTY()
	USBPlayerOverlayWidget* PlayerOverlay = nullptr;

public:
	FORCEINLINE USBPlayerOverlayWidget* GetPlayerOverlay() const
	{
		return PlayerOverlay;
	}
};
