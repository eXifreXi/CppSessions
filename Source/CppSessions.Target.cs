using UnrealBuildTool;

public class CppSessionsTarget : TargetRules
{
	public CppSessionsTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "CppSessions" } );
	}
}