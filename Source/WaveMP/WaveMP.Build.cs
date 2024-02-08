// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WaveMP : ModuleRules
{
	public WaveMP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}
