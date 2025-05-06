// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAM415Reaban : ModuleRules
{
	public GAM415Reaban(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
