#include "BuildSystem/FacilityPort.h"

void AFacilityPort::Tick(float Delta)
{
	Super::Tick(Delta);

	// TODO: ���⼭ �ϸ� �ȵ�...
	UpdateToken();
}

void AFacilityPort::UpdateToken()
{
	if (Token > 0)
	{
		Token = (Token + 1) % 3;
	}
}

