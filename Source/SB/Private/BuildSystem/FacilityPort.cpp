#include "BuildSystem/FacilityPort.h"
#include "Components/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "SB/DebugMacro.h"

AFacilityPort::AFacilityPort()
{
	BuildBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFacilityPort::Tick(float Delta)
{
	Super::Tick(Delta);

	// TODO: 여기서 하면 안됨...
	UpdateToken();
}

void AFacilityPort::BeginPlay()
{
	Super::BeginPlay();

	SetOwner(GetAttachParentActor());
	BuildBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFacilityPort::UpdateToken()
{
	if (Token > 0)
	{
		Token = (Token + 1) % 3;
	}
}

