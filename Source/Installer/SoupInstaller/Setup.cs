using System;
using WixSharp;

class Script
{
	static public void Main()
	{
		var soupVersion = new Version(0, 37, 1);

		var soupOutFolder = "../../../out";
		var soupRunFolder = $"{soupOutFolder}/run";
		var project = new Project(
			"Soup Build",
			new Dir(
				@"%ProgramFiles%\SoupBuild\Soup",
				new Files($"{soupRunFolder}/*.*")),
			new EnvironmentVariable("PATH", "[INSTALLDIR]")
			{
				System = true,
				Permanent = false,
				Part = EnvVarPart.last,
			});

		project.Platform = Platform.x64;
		project.GUID = new Guid("2CF2A792-A266-416D-ACF9-B81781DC2B76");
		project.BackgroundImage = @"Images\DialogBackground.bmp";
		project.BannerImage = @"Images\DialogBanner.bmp";
		project.LicenceFile = @"License.rtf";
		project.OutDir = soupOutFolder;
		project.OutFileName = $"soup-build-{soupVersion}-windows-x64";

		// Prevent two installations at the same time
		project.MajorUpgrade = new MajorUpgrade()
		{
			Schedule = UpgradeSchedule.afterInstallInitialize,
			DowngradeErrorMessage = "A later version of [ProductName] is already installed. Setup will now exit.",
		};

		// Upgrade values
		project.Version = soupVersion;

		Compiler.BuildMsi(project);
	}
}