// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrucibleTests : ModuleRules
{
	public CrucibleTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"Crucible"
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"CrucibleTests",
			"CrucibleTests/Helpers",
			"CrucibleTests/Tests"
		});
	}
}
