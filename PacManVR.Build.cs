using UnrealBuildTool;

public class PacManVR : ModuleRules
{
	public PacManVR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] { "AudioCapture", "Amf", "AudioMixer", "AudioCaptureCore" });
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Http", "Json" });
		
	}
}