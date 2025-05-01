// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/ConveyorBelt.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Items/PackagedItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SB/DebugMacro.h"

AConveyorBelt::AConveyorBelt()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(GetRootComponent());
	SplineComponent->SetRelativeLocation(FVector(-50, 0, -24.99));
}

void AConveyorBelt::Tick(float Delta)
{
	Super::Tick(Delta);

	TraceReceiver();
	TraceSupplier();
	CarryPackage(Delta);
}

void AConveyorBelt::CarryPackage(float Delta)
{
	if (CarriedPackage)
	{
		ElapsedMoveTime += Delta;
		if (ElapsedMoveTime < MoveDuration)
		{
			float Intp = FMath::Lerp(0.0f, SplineComponent->GetSplineLength(), ElapsedMoveTime / MoveDuration);
			FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Intp, ESplineCoordinateSpace::World);
			FRotator Rotator = SplineComponent->GetRotationAtDistanceAlongSpline(Intp, ESplineCoordinateSpace::World);

			CarriedPackage->SetActorLocationAndRotation(Location, Rotator);
		}
		else
		{
			TrySupplyPackage();
		}
	}
	else
	{
		TryReceivePackage();
	}
}

void AConveyorBelt::TrySupplyPackage()
{
	if (IsValid(Receiver))
	{
		if (Receiver->ReceivePackage(CarriedPackage))
		{
			SupplyPackage();
			TryReceivePackage();
		}
	}
}

void AConveyorBelt::TryReceivePackage()
{
	if (IsValid(Supplier))
	{
		ReceivePackage(Supplier->SupplyPackage());
	}
}

void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
}

bool AConveyorBelt::ReceivePackage(APackagedItem* Package)
{
	if (Package && CarriedPackage == nullptr && Package != CarriedPackage)
	{
		CarriedPackage = Package;
		ElapsedMoveTime = 0.0f;
		return true;
	}
	return false;
}

APackagedItem* AConveyorBelt::SupplyPackage()
{
	if (ElapsedMoveTime >= MoveDuration)
	{
		APackagedItem* Supplement = CarriedPackage;
		CarriedPackage = nullptr;
		ElapsedMoveTime = 0.0f;
		return Supplement;
	}
	return nullptr;
}

void AConveyorBelt::SetAsPreview()
{
	Super::SetAsPreview();

	TArray<USceneComponent*> ChildsOfSpline;
	SplineComponent->GetChildrenComponents(true, ChildsOfSpline);
	for (USceneComponent* Child : ChildsOfSpline)
	{
		USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Child);
		if (SplineMesh)
		{
			SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AConveyorBelt::SetAllMaterials(UMaterialInterface* Material)
{
	Super::SetAllMaterials(Material);
	TArray<USceneComponent*> ChildsOfSpline;
	SplineComponent->GetChildrenComponents(true, ChildsOfSpline);
	for (USceneComponent* Child : ChildsOfSpline)
	{
		USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Child);
		if (SplineMesh)
		{
			int32 MaterialNum = SplineMesh->GetNumMaterials();
			for (int32 i = 0; i < MaterialNum; ++i)
			{
				SplineMesh->SetMaterial(i, Material);
			}
		}
	}
}

void AConveyorBelt::TraceReceiver()
{
	float SplineLength = SplineComponent->GetSplineLength();
	FVector Start = SplineComponent->GetLocationAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World);
	FVector Direction = Start - GetActorLocation();
	Direction.Z = 0.0f;
	Direction.Normalize();
	FVector End = Start + Direction;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<FHitResult> HitResults;
	TraceBuildings(Start, End, HitResults);

	for (const auto& HitResult : HitResults)
	{
		Receiver = Cast<AConveyorBelt>(HitResult.GetActor());
		if (Receiver)
		{
			return;
		}
	}
}

void AConveyorBelt::TraceSupplier()
{
	FVector Start = SplineComponent->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
	FVector Direction = Start - GetActorLocation();
	Direction.Z = 0.0f;
	Direction.Normalize();
	FVector End = Start + Direction;
	TArray<FHitResult> HitResults;

	TraceBuildings(Start, End, HitResults);

	TArray<AActor*> Actors;
	for (const auto& HitResult : HitResults)
	{
		Supplier = Cast<AConveyorBelt>(HitResult.GetActor());
		if (Supplier)
		{
			return;
		}
	}
}
