// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorModeTestPluginEditorModeToolkit.h"
#include "EditorModeTestPluginEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "EditorModeTestPluginEditorModeToolkit"

FEditorModeTestPluginEditorModeToolkit::FEditorModeTestPluginEditorModeToolkit()
{
}

void FEditorModeTestPluginEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FEditorModeTestPluginEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FEditorModeTestPluginEditorModeToolkit::GetToolkitFName() const
{
	return FName("EditorModeTestPluginEditorMode");
}

FText FEditorModeTestPluginEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "EditorModeTestPluginEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
