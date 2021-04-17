namespace UnrealBuildTool.Rules
{
	public class UT4XKickIdlers : ModuleRules
	{
		public UT4XKickIdlers(TargetInfo Target)
        {
			PrivateIncludePaths.Add("UT4XKickIdlers/Private");

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "UnrealTournament"
                }
			);
        }
	}
}