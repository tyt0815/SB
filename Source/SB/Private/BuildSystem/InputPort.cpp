#include "BuildSystem/InputPort.h"
#include "Components/InventoryComponent.h"
#include "SB/DebugMacro.h"

void AInputPort::TrySupplyPackage()
{
	if (CarriedPackage && ConnectedInventory && Token == 0 && ConnectedInventory->AddItem(CarriedPackage))
	{
		++Token;
		SupplyPackage();
		TryReceivePackage();
	}
}

void AInputPort::AddInteractions()
{
}
