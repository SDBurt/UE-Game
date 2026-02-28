// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Crucible : ModuleRules
{
	public Crucible(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UMG"
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"Crucible",
			"Crucible/Core",
			"Crucible/Character",
			"Crucible/Combat",
			"Crucible/Data",
			"Crucible/Tags"
		});
	}
}
