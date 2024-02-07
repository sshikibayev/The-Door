// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TheDoor : ModuleRules
{
	public TheDoor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "MultiplayerSessions", "OnlineSubsystem", "OnlineSubsystemSteam", "UMG", "Slate", "SlateCore" });
    }
}
