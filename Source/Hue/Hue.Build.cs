// Copyright Mippithedork 2026, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Hue : ModuleRules
{
    public Hue(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "DeveloperSettings"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AppFramework",
                "BlueprintGraph",
                "Engine",
                "GraphEditor",
                "Kismet",
                "Settings",
                "Slate",
                "SlateCore",
                "ToolMenus",
                "UnrealEd"
            }
        );
    }
}
