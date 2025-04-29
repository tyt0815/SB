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

	ProductionInputs->SetInventorySize(FMath::Clamp(ProductionInputs->GetInventorySize(), 1, InputPortGridCoord.Num()));
	ProductionOutputs->SetInventorySize(1);

	InitializePorts();
}

void AProductionFacility::InitializePorts()
{
	// DestroyAllPorts();

	AdjustChildComponents(InputPortComponents, InputPortGridCoord.Num());
	AdjustChildComponents(OutputPortComponents, OutputPortGridCoord.Num());
	

	// 컴포넌트 설정
	for (int i = 0; i < InputPortComponents.Num(); ++i)
	{
		UChildActorComponent* InputPortComponent = InputPortComponents[i];
		if (InputPortComponent)
		{
			InputPortComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			InputPortComponent->SetRelativeLocation(BuildSystem::GetRelativeLocation(InputPortGridCoord[i]));
			if (InputPortClass != InputPortComponent->GetChildActorClass())
			{
				InputPortComponent->DestroyChildActor();
				InputPortComponent->SetChildActorClass(InputPortClass);
			}

			AInputPort* InputPort = Cast<AInputPort>(InputPortComponent->GetChildActor());
			if (InputPort)
			{
				InputPort->SetConnectedInventory(ProductionInputs);
			}
		}
	}
	for (int i = 0; i < OutputPortComponents.Num(); ++i)
	{
		UChildActorComponent* OutputPortComponent = OutputPortComponents[i];
		if (OutputPortComponent)
		{
			OutputPortComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
			OutputPortComponent->SetRelativeLocation(BuildSystem::GetRelativeLocation(OutputPortGridCoord[i]));
			if (OutputPortClass != OutputPortComponent->GetChildActorClass())
			{
				OutputPortComponent->DestroyChildActor();
				OutputPortComponent->SetChildActorClass(OutputPortClass);
			}
			AOutputPort* OutputPort = Cast<AOutputPort>(OutputPortComponent->GetChildActor());
			if (OutputPort)
			{
				OutputPort->SetConnectedInventory(ProductionOutputs);
			}
		}
	}
}

void AProductionFacility::DestroyAllPorts()
{
	for (UChildActorComponent* Inputport : InputPortComponents)
	{
		Inputport->DestroyComponent();
	}
	InputPortComponents.Empty();

	for (UChildActorComponent* Outputport : InputPortComponents)
	{
		Outputport->DestroyComponent();
	}
	InputPortComponents.Empty();
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
