// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorModeTestPluginEditorMode.h"
#include "EditorModeTestPluginEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "EditorModeTestPluginEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/EditorModeTestPluginSimpleTool.h"
#include "Tools/EditorModeTestPluginInteractiveTool.h"

// step 2: register a ToolBuilder in FEditorModeTestPluginEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "EditorModeTestPluginEditorMode"

const FEditorModeID UEditorModeTestPluginEditorMode::EM_EditorModeTestPluginEditorModeId = TEXT("EM_EditorModeTestPluginEditorMode");

FString UEditorModeTestPluginEditorMode::SimpleToolName = TEXT("EditorModeTestPlugin_ActorInfoTool");
FString UEditorModeTestPluginEditorMode::InteractiveToolName = TEXT("EditorModeTestPlugin_MeasureDistanceTool");


UEditorModeTestPluginEditorMode::UEditorModeTestPluginEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UEditorModeTestPluginEditorMode::EM_EditorModeTestPluginEditorModeId,
		LOCTEXT("ModeName", "EditorModeTestPlugin"),
		FSlateIcon(),
		true);
}


UEditorModeTestPluginEditorMode::~UEditorModeTestPluginEditorMode()
{
}


void UEditorModeTestPluginEditorMode::ActorSelectionChangeNotify()
{
}

void UEditorModeTestPluginEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FEditorModeTestPluginEditorModeCommands& SampleToolCommands = FEditorModeTestPluginEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UEditorModeTestPluginSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UEditorModeTestPluginInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UEditorModeTestPluginEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FEditorModeTestPluginEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UEditorModeTestPluginEditorMode::GetModeCommands() const
{
	return FEditorModeTestPluginEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
