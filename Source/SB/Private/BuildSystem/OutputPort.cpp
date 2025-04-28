#include "BuildSystem/OutputPort.h"
#include "Items/PackagedItem.h"
#include "Components/InventoryComponent.h"
#include "GameInstances/SBGameInstance.h"

AOutputPort::AOutputPort()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOutputPort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOutputPort::BeginPlay()
{
	Super::BeginPlay();
}

APackagedItem* AOutputPort::SupplyPackage_Implementation()
{
	APackagedItem* PackagedItem = nullptr;
	UWorld* World = GetWorld();
	if (ConnectedInventory && Token == 0 && World && ConnectedInventory->IsValidIndex(LinkedItemDataIndex))
	{
		FItemData ItemData = ConnectedInventory->RemoveItem(LinkedItemDataIndex, 1);
		if (ItemData.Quantity > 0)
		{
			USBGameInstance* GameInstance = Cast<USBGameInstance>(World->GetGameInstance());
			if (GameInstance)
			{
				PackagedItem = World->SpawnActor<APackagedItem>(GameInstance->GetPackagedItemClass());
				if (PackagedItem)
				{
					Token = 1;
				}
			}
		}
	}
	return PackagedItem;
}
