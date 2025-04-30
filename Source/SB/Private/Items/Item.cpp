#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "SB/DebugMacro.h"
#include "Characters/Player/SBPlayer.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	InitMeshsVisibilityAndPhysics();

	AddInteractions();
}

FItemData AItem::MakeItemData()
{
	FItemData ItemInfo;
	ItemInfo.ItemClass = GetClass();
	ItemInfo.Name = GetItemName();
	ItemInfo.bStackable = IsStackable();
	ItemInfo.Thumbnail = GetThumnail();
	ItemInfo.Quantity = 1;
	return ItemInfo;
}

void AItem::InitMeshsVisibilityAndPhysics()
{
	if (!bSimulatePhysics)
	{
		return;
	}
	if (StaticMesh && StaticMesh->GetStaticMesh())
	{
		ActivateStaticMesh(true);
	}
	else
	{
		ActivateStaticMesh(false);
	}
	if (SkeletalMesh && SkeletalMesh->GetSkeletalMeshAsset())
	{
		ActivateSkeletalMesh(true);
	}
	else
	{
		ActivateSkeletalMesh(false);
	}
}

void AItem::AddInteractions()
{
	int i = InteractionComponent->AddInteraction("Pick Up");
	InteractionComponent->AddInteractionAt(i, this, &AItem::AddToInventory);
}

void AItem::AddToInventory(AActor* OtherActor)
{
	if (OtherActor)
	{
		UInventoryComponent* Inventory = OtherActor->GetComponentByClass<UInventoryComponent>();
		if (Inventory)
		{
			if (Inventory->AddItem(this))
			{
				Destroy();
			}
		}
	}
}

void AItem::ActivateStaticMesh(bool bActive)
{
	if (StaticMesh)
	{
		StaticMesh->SetVisibility(bActive);
		StaticMesh->SetSimulatePhysics(bActive);
	}
}

void AItem::ActivateSkeletalMesh(bool bActive)
{
	if (SkeletalMesh)
	{
		SkeletalMesh->SetVisibility(bActive);
		SkeletalMesh->SetSimulatePhysics(bActive);
	}
}
