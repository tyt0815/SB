// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorModeTestPluginModule.h"
#include "EditorModeTestPluginEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "EditorModeTestPluginModule"

void FEditorModeTestPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FEditorModeTestPluginEditorModeCommands::Register();
}

void FEditorModeTestPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FEditorModeTestPluginEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorModeTestPluginModule, EditorModeTestPluginEditorMode)