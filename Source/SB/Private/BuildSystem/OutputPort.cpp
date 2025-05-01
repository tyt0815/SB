#include "BuildSystem/OutputPort.h"
#include "BuildSystem/ProductionFacility.h"
#include "Items/PackagedItem.h"
#include "Components/InventoryComponent.h"
#include "Components/InteractionComponent.h"
#include "Characters/Player/SBPlayer.h"
#include "GameInstances/SBGameInstance.h"
#include "SB/DebugMacro.h"

void AOutputPort::Tick(float Delta)
{
	Super::Tick(Delta);
}

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
	if (Receiver == nullptr)
	{
		return;
	}
	if (!bInteractive)
	{
		AProductionFacility* LinkedFacility = Cast<AProductionFacility>(GetOwner());
		if (LinkedFacility)
		{
			LinkedItemClass = LinkedFacility->GetRemainedItemData()->ItemClass;
		}
	}
	UWorld* World = GetWorld();
	int32 Index = ConnectedInventory->FindItemIndex(LinkedItemClass);
	if (Token == 0 && World && Index >= 0)
	{
		FItemData ItemData = ConnectedInventory->RemoveItem(Index, 1);
		if (ItemData.Quantity > 0)
		{
			USBGameInstance* GameInstance = Cast<USBGameInstance>(World->GetGameInstance());
			if (GameInstance)
			{
				APackagedItem* Package = World->SpawnActor<APackagedItem>(GameInstance->GetPackagedItemClass());
				if (Package)
				{
					Package->SetItemData(ItemData);
					if (ReceivePackage(Package))
					{
						Token = 1;
					}
				}
			}
		}
	}
	
}

FItemData* AOutputPort::GetLinkedItemData()
{
	int32 Index = ConnectedInventory->FindItemIndex(LinkedItemClass);
	if (Index >= 0)
	{
		return ConnectedInventory->GetItemDataPtr(Index);
	}
	return nullptr;
}

void AOutputPort::SetLinkedItemClass(TSubclassOf<AItem> ItemClass)
{
	LinkedItemClass = ItemClass;
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
		Player->OpenHUBOutputPortInfoWidget(ConnectedInventory, this);
	}
}