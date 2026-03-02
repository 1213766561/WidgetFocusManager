// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WidgetFocusManager : ModuleRules
{
	public WidgetFocusManager(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"Runtime/UMG",
                "Runtime/Slate",
                "Runtime/SlateCore",
			}
		);
				
		PrivateIncludePaths.AddRange(
			new string[] {
			}
		);
			
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"UMG", // <<--- 添加 UMG，因为 Public 头文件使用了 UUserWidget/UWidget/ESlateVisibility
			}
		);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}
