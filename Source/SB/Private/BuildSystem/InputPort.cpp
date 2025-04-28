#include "BuildSystem/InputPort.h"
#include "Components/InventoryComponent.h"

AInputPort::AInputPort()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInputPort::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AInputPort::ReceivePackage_Implementation(APackagedItem* Package)
{
	if (Package && ConnectedInventory && Token == 0 && ConnectedInventory->AddItem(Package))
	{
		++Token;
		return true;
	}
	return false;
}

// Called every frame
void AInputPort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

