#include "BuildSystem/OutputPort.h"
#include "Items/PackagedItem.h"
#include "Components/InventoryComponent.h"
#include "GameInstances/SBGameInstance.h"

void AOutputPort::TryReceivePackage()
{
	UWorld* World = GetWorld();
	if (ConnectedInventory && Token == 0 && World && ConnectedInventory->IsValidIndex(LinkedItemDataIndex))
	{
		FItemData ItemData = ConnectedInventory->RemoveItem(LinkedItemDataIndex, 1);
		if (ItemData.Quantity > 0)
		{
			USBGameInstance* GameInstance = Cast<USBGameInstance>(World->GetGameInstance());
			if (GameInstance)
			{
				if (ReceivePackage(World->SpawnActor<APackagedItem>(GameInstance->GetPackagedItemClass())))
				{
					Token = 1;
				}
			}
		}
	}
}