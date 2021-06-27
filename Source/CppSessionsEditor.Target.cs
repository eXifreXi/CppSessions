using UnrealBuildTool;

public class CppSessionsEditorTarget : TargetRules
{
	public CppSessionsEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "CppSessions" } );
	}
}