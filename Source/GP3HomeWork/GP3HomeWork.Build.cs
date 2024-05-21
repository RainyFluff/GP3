// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GP3HomeWork : ModuleRules
{
	public GP3HomeWork(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
