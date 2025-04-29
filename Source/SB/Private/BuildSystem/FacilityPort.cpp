#include "BuildSystem/FacilityPort.h"

void AFacilityPort::Tick(float Delta)
{
	Super::Tick(Delta);

	// TODO: 여기서 하면 안됨...
	UpdateToken();
}

void AFacilityPort::UpdateToken()
{
	if (Token > 0)
	{
		Token = (Token + 1) % 3;
	}
}

