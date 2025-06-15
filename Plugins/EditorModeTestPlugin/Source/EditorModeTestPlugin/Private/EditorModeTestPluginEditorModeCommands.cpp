// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorModeTestPluginEditorModeCommands.h"
#include "EditorModeTestPluginEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "EditorModeTestPluginEditorModeCommands"

FEditorModeTestPluginEditorModeCommands::FEditorModeTestPluginEditorModeCommands()
	: TCommands<FEditorModeTestPluginEditorModeCommands>("EditorModeTestPluginEditorMode",
		NSLOCTEXT("EditorModeTestPluginEditorMode", "EditorModeTestPluginEditorModeCommands", "EditorModeTestPlugin Editor Mode"),
		NAME_None,
		FEditorStyle::GetStyleSetName())
{
}

void FEditorModeTestPluginEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(InteractiveTool, "Measure Distance", "Measures distance between 2 points (click to set origin, shift-click to set end point)", EUserInterfaceActionType::ToggleButton, FInputChord());
	ToolCommands.Add(InteractiveTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FEditorModeTestPluginEditorModeCommands::GetCommands()
{
	return FEditorModeTestPluginEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
