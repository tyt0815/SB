#include "HUDs/SBHUD.h"
#include "HUDs/SBPlayerOverlayWidget.h"

void ASBHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			if (PlayerOverlayClass)
			{
				PlayerOverlay = CreateWidget<USBPlayerOverlayWidget>(Controller, PlayerOverlayClass);
				PlayerOverlay->AddToViewport();
			}
		}
	}
}