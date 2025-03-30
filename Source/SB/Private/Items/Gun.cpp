// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Gun.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"

AGun::AGun()
{
	UArrowComponent* ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	SetRootComponent(ArrowComponent);
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollisionComponent->SetupAttachment(SkeletalMeshComponent);
}
