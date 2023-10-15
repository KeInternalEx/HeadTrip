using UnrealBuildTool;
 
public class HtCore : ModuleRules
{
	public HtCore(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "NetCore", "UMG" });
 
		PublicIncludePaths.AddRange(new string[] {"HtCore/Public"});
		PrivateIncludePaths.AddRange(new string[] {"HtCore/Private"});
	}
}