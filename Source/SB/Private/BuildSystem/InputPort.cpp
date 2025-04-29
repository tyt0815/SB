#include "BuildSystem/InputPort.h"
#include "Components/InventoryComponent.h"
#include "SB/DebugMacro.h"

AInputPort::AInputPort()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInputPort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInputPort::BeginPlay()
{
	Super::BeginPlay();
	
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