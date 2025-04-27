#include "BuildSystem/ProductionFacility.h"
#include "Components/InventoryComponent.h"
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

	InputPortNum = FMath::Clamp(InputPortNum, 1, CellExtent.X);
	OutputPortNum = FMath::Clamp(OutputPortNum, 1, CellExtent.X);
	ProductionInputs->SetInventorySize(FMath::Clamp(ProductionInputs->GetInventorySize(), 1, InputPortNum));
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
	StartProduction();
}

void AProductionFacility::AddProductionInput(FItemData ItemData)
{
	ProductionInputs->AddItem(ItemData);

	StartProduction();
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
		// Input�� �´� �����ǰ� �ִ����� ã�´�.
		for (const auto& Ingrediant : Recipe.Ingrediant)
		{
			const TSubclassOf<AItem>& IngrediantClass = Ingrediant.Key;
			const int32 IngrediantQuantity = Ingrediant.Value;

			bool bFind = false;
			// �������� ��ᰡ Input�� �ִ��� Ȯ���Ѵ�.
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
