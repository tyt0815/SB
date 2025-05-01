#include "BuildSystem/ProductionFacility.h"
#include "Components/InventoryComponent.h"
#include "Components/InteractionComponent.h"
#include "Characters/Player/SBPlayer.h"
#include "BuildSystem/InputPort.h"
#include "BuildSystem/OutputPort.h"
#include "BuildSystem/BuildSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "SB/DebugMacro.h"

AProductionFacility::AProductionFacility()
{
	ProductionInputs = CreateDefaultSubobject<UInventoryComponent>(TEXT("ProductionInputs"));
	ProductionInputs->SetInventorySize(3);
	ProductionInputs->ComponentTags.Add("Inputs");
	ProductionOutputs = CreateDefaultSubobject<UInventoryComponent>(TEXT("ProductionOutputs"));
	ProductionOutputs->SetInventorySize(1);
	ProductionOutputs->ComponentTags.Add("Outputs");

	CellExtent = FIntVector(2, 2, 2);
}

void AProductionFacility::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ProductionInputs->SetInventorySize(FMath::Clamp(ProductionInputs->GetInventorySize(), 1, InputPortGridCoord.Num()));
	ProductionOutputs->SetInventorySize(1);
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
}

void AProductionFacility::BeginDestroy()
{
	Super::BeginDestroy();
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
			if (Temp)
			{
				OutputData = Temp->MakeItemData();
				Temp->Destroy();
				bProducted = true;
				ProductionTimeLeft = Recipe.ProductionTime;
			}
		}
	}
}

FItemData* AProductionFacility::GetRemainedItemData()
{
	return ProductionOutputs->GetItemDataPtr(0);
}

void AProductionFacility::AddInteractions()
{
	int32 Index = InteractionComponent->AddInteraction("Info");
	InteractionComponent->AddInteractionAt(Index, this, &AProductionFacility::ShowInfoWidget)
	Super::AddInteractions();
}

void AProductionFacility::ShowInfoWidget(AActor* OtherActor)
{
	ASBPlayer* Player = Cast<ASBPlayer>(OtherActor);
	if (Player)
	{
		Player->OpenProductionFacilityInfoWidget(ProductionInputs, ProductionOutputs);
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
