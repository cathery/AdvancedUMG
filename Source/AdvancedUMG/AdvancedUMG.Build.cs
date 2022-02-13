// Copyright (c) 2022 cathery

using UnrealBuildTool;

public class AdvancedUMG : ModuleRules
{
	public AdvancedUMG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"InputCore",
			"UMG",
		});
	}
}
