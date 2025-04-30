#include "BuildSystem/OutputPort.h"
#include "Items/PackagedItem.h"
#include "Components/InventoryComponent.h"
#include "Components/InteractionComponent.h"
#include "Characters/Player/SBPlayer.h"
#include "GameInstances/SBGameInstance.h"
#include "SB/DebugMacro.h"

void AOutputPort::BeginPlay()
{
	Super::BeginPlay();

	if (Owner)
	{
		UInventoryComponent* InventoryComponent = Owner->FindComponentByTag<UInventoryComponent>("Outputs");
		if (InventoryComponent == nullptr)
		{
			InventoryComponent = Owner->FindComponentByTag<UInventoryComponent>("Storage");
		}
		SetConnectedInventory(InventoryComponent);
	}
}

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

void AOutputPort::AddInteractions()
{
	if (bInteractive)
	{
		int Index = InteractionComponent->AddInteraction("Info");
		InteractionComponent->AddInteractionAt(Index, this, &AOutputPort::ShowInfo);
	}
}

void AOutputPort::ShowInfo(AActor* Actor)
{
	ASBPlayer* Player = Cast<ASBPlayer>(Actor);
	if (Player)
	{
		FItemData* ItemData = nullptr;
		if (ConnectedInventory->IsValidIndex(LinkedItemDataIndex))
		{
			ItemData = ConnectedInventory->GetItemDataPtr(LinkedItemDataIndex);
		}
		Player->OpenHUBOutputPortInfoWidget(ConnectedInventory, ItemData);
	}
}