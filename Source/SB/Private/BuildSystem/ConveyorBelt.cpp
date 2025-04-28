// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/ConveyorBelt.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Items/PackagedItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SB/DebugMacro.h"

AConveyorBelt::AConveyorBelt()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(GetRootComponent());
	SplineComponent->SetRelativeLocation(FVector(-50, 0, 0));
}

void AConveyorBelt::Tick(float Delta)
{
	Super::Tick(Delta);

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
	AActor* Receiver = TraceReceiver();
	if (Receiver)
	{
		if (IPackageReceiver::Execute_ReceivePackage(Receiver, CarriedPackage))
		{
			IPackageSupplier::Execute_SupplyPackage(this);
			TryReceivePackage();
		}
	}
}

void AConveyorBelt::TryReceivePackage()
{
	AActor* Supplier = TraceSupplier();
	if (Supplier)
	{
		IPackageReceiver::Execute_ReceivePackage(this, IPackageSupplier::Execute_SupplyPackage(Supplier));
	}
}

void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
}

bool AConveyorBelt::ReceivePackage_Implementation(APackagedItem* Package)
{
	if (Package && CarriedPackage == nullptr && Package != CarriedPackage)
	{
		CarriedPackage = Package;
		ElapsedMoveTime = 0.0f;
		return true;
	}
	return false;
}

APackagedItem* AConveyorBelt::SupplyPackage_Implementation()
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

AConveyorBelt* AConveyorBelt::TraceNextConveyorBelt()
{
	float SplineLength = SplineComponent->GetSplineLength();
	FVector Start = SplineComponent->GetLocationAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World);
	FRotator Rotator = SplineComponent->GetRotationAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World);
	FVector End = Start + Rotator.Vector();
	FHitResult HitResult;
	TraceBuilding(Start, End, HitResult);
	return Cast<AConveyorBelt>(HitResult.GetActor());
}

AConveyorBelt* AConveyorBelt::TracePrevConveyorBelt()
{
	FVector Start = SplineComponent->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
	FRotator Rotator = SplineComponent->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
	FVector End = Start - Rotator.Vector();
	FHitResult HitResult;
	TraceBuilding(Start, End, HitResult);
	return Cast<AConveyorBelt>(HitResult.GetActor());
}

AActor* AConveyorBelt::TraceReceiver()
{
	float SplineLength = SplineComponent->GetSplineLength();
	FVector Start = SplineComponent->GetLocationAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World);
	FRotator Rotator = SplineComponent->GetRotationAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World);
	FVector End = Start + Rotator.Vector();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FVector BoxExtent = BuildBlocker->GetScaledBoxExtent();
	BoxExtent.X = 0.0f;

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMulti(
		this,
		Start,
		End,
		BoxExtent,
		Rotator,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	TArray<AActor*> Actors;
	for (const auto& HitResult : HitResults)
	{
		AActor* Candidate = HitResult.GetActor();
		if (Candidate && Candidate->Implements<UPackageReceiver>())
		{
			SCREEN_LOG_SINGLE_FRAME(Candidate->GetName());
			return Candidate;
		}
	}
	
	return nullptr;
}

AActor* AConveyorBelt::TraceSupplier()
{
	FVector Start = SplineComponent->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
	FRotator Rotator = SplineComponent->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
	FVector End = Start - Rotator.Vector();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FVector BoxExtent = BuildBlocker->GetScaledBoxExtent();
	BoxExtent.X = 0.0f;

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMulti(
		this,
		Start,
		End,
		BoxExtent,
		Rotator,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	TArray<AActor*> Actors;
	for (const auto& HitResult : HitResults)
	{
		AActor* Candidate = HitResult.GetActor();
		if (Candidate && Candidate->Implements<UPackageSupplier>())
		{
			SCREEN_LOG_SINGLE_FRAME(Candidate->GetName());
			return Candidate;
		}
	}

	return nullptr;
}
