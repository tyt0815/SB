#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "SB/DebugMacro.h"
#include "Characters/Player/SBPlayer.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractionRange = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRange"));
	SetRootComponent(InteractionRange);
	InteractionRange->SetCollisionProfileName(TEXT("InteractionRange"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());
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

	InteractionRange->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnInteractionRangeBeginOverlap);
}

void AItem::OnInteractionRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASBPlayer* Player = Cast<ASBPlayer>(OtherActor);
	if (Player)
	{
		
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
