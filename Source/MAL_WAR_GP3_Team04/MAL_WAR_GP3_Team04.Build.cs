// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MAL_WAR_GP3_Team04 : ModuleRules
{
	public MAL_WAR_GP3_Team04(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayCameras" });
	}
}
