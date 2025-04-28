#include "BuildSystem/ProductionFacility.h"
#include "Components/InventoryComponent.h"
#include "BuildSystem/InputPort.h"
#include "BuildSystem/OutputPort.h"
#include "BuildSystem/BuildSystem.h"
#include "SB/DebugMacro.h"

AProductionFacility::AProductionFacility()
{
	ProductionInputs = CreateDefaultSubobject<UInventoryComponent>(TEXT("ProductionInputs"));
	ProductionInputs->SetInventorySize(3);
	ProductionOutputs = CreateDefaultSubobject<UInventoryComponent>(TEXT("ProductionOutputs"));
	ProductionOutputs->SetInventorySize(1);

	CellExtent = FIntVector(3, 3, 3);
}

void AProductionFacility::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AProductionFacility::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsOperating())
	{
		if (bProducted)
		{
			if (ProductionTimeLeft <= 0)
			{
				// TODO
				TryAddToProductionOutput();
			}
			else
			{
				ProductionTimeLeft -= Delta;
			}
		}
		else
		{
			StartProduction();
		}
	}
}

void AProductionFacility::BeginPlay()
{
	Super::BeginPlay();

	if (Recipes.IsValidIndex(0))
	{
		for (auto a : Recipes[0].Ingrediant)
		{
			FItemData ItemData;
			ItemData.bStackable = true;
			ItemData.Name = "sibal";
			ItemData.ItemClass = a.Key;
			ItemData.Quantity = a.Value * 3;
			ProductionInputs->AddItem(ItemData);
		}
	}

	ProductionInputs->SetInventorySize(FMath::Clamp(ProductionInputs->GetInventorySize(), 1, InputPortGridCoord.Num()));
	ProductionOutputs->SetInventorySize(1);

	UWorld* World = GetWorld();
	if (World)
	{
		if (InputPortClass)
		{
			for (const FIntVector GridCoord : InputPortGridCoord)
			{
				AInputPort* Inputport = World->SpawnActor<AInputPort>(InputPortClass);
				if (Inputport)
				{
					InputPorts.Add(Inputport);
					Inputport->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
					SCREEN_LOG(1, Inputport->GetActorLocation().ToString());
					Inputport->SetActorRelativeLocation(BuildSystem::GetRelativeLocation(GridCoord));
					SCREEN_LOG(2, Inputport->GetActorLocation().ToString());
				}
			}
		}
		if (OutputPortClass)
		{
			for (const FIntVector GridCoord : OutputPortGridCoord)
			{
				AOutputPort* OutputPort = World->SpawnActor<AOutputPort>(OutputPortClass);
				if (OutputPort)
				{
					OutputPorts.Add(OutputPort);
					OutputPort->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
					OutputPort->SetActorRelativeLocation(BuildSystem::GetRelativeLocation(GridCoord));
				}
			}
		}
	}
}

void AProductionFacility::StartProduction()
{
	UWorld* World = GetWorld();
	if (!World || bProducted)
	{
		return;
	}
	

	const TArray<FItemData>& Inputs = ProductionInputs->GetInventory();

	for (const FProductionRecipe& Recipe : Recipes)
	{
		bool bProductable = true;
		TMap<TSubclassOf<AItem>, int32> ItemIndices;
		// Input에 맞는 레시피가 있는지를 찾는다.
		for (const auto& Ingrediant : Recipe.Ingrediant)
		{
			const TSubclassOf<AItem>& IngrediantClass = Ingrediant.Key;
			const int32 IngrediantQuantity = Ingrediant.Value;

			bool bFind = false;
			// 레시피의 재료가 Input에 있는지 확인한다.
			for (int i = 0; i < Inputs.Num(); ++i)
			{
				if (Inputs[i].ItemClass == IngrediantClass && Inputs[i].Quantity >= IngrediantQuantity)
				{
					bFind = true;
					ItemIndices.Add(IngrediantClass, i);
					break;
				}
			}
			if (bFind)
			{
				continue;
			}
			else
			{
				bProductable = false;
				break;
			}
		}

		if(bProductable)
		{
			for (const auto& Ingrediant : Recipe.Ingrediant)
			{
				const TSubclassOf<AItem>& IngrediantClass = Ingrediant.Key;
				const int32 IngrediantQuantity = Ingrediant.Value;
				int32 ItemIndex = ItemIndices[IngrediantClass];
				ProductionInputs->RemoveItem(ItemIndex, IngrediantQuantity);
			}

			AItem* Temp = World->SpawnActor<AItem>(Recipe.OutputItemClass);
			OutputData = Temp->MakeItemData();
			Temp->Destroy();

			bProducted = true;
			ProductionTimeLeft = Recipe.ProductionTime;
			return;
		}
	}
}

void AProductionFacility::TryAddToProductionOutput()
{
	if (ProductionOutputs->AddItem(OutputData))
	{
		bProducted = false;
		StartProduction();
	}
}
