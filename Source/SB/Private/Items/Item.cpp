#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Components/InteractionComponent.h"
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

	AddInteractions();
}

void AItem::AddInteractions()
{
	int i = InteractionComponent->AddInteraction("Pick Up");
	InteractionComponent->AddInteractionAt(i,this, &AItem::AddToInventory);
	i = InteractionComponent->AddInteraction("Test");
}

void AItem::AddToInventory(AActor* OtherActor)
{
	SCREEN_LOG_NONE_KEY(OtherActor->GetName());
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
