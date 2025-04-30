#include "BuildSystem/InputPort.h"
#include "Components/InventoryComponent.h"
#include "SB/DebugMacro.h"

void AInputPort::BeginPlay()
{
	Super::BeginPlay();

	if (Owner)
	{
		UInventoryComponent* InventoryComponent = Owner->FindComponentByTag<UInventoryComponent>("Inputs");
		if (InventoryComponent == nullptr) 
		{
			InventoryComponent = Owner->FindComponentByTag<UInventoryComponent>("Storage");
		}
		SetConnectedInventory(InventoryComponent);
	}
}

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
