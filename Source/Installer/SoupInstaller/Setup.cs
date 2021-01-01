using System;
using WixSharp;

class Script
{
	static public void Main()
	{
		var soupBinFolder = @"..\..\..\out\Soup\MSVC\release\win32\x64\bin\";
		var project = new Project(
			"Soup",
			new Dir(
				@"%ProgramFiles%\SoupBuild\Soup",
				new File(System.IO.Path.Combine(soupBinFolder, "copy.exe")),
				new File(System.IO.Path.Combine(soupBinFolder, "libcrypto-3.dll")),
				new File(System.IO.Path.Combine(soupBinFolder, "libssl-3.dll")),
				new File(System.IO.Path.Combine(soupBinFolder, "LocalUserConfig.json")),
				new File(System.IO.Path.Combine(soupBinFolder, "mkdir.exe")),
				new File(System.IO.Path.Combine(soupBinFolder, "Monitor.Detours.32.dll")),
				new File(System.IO.Path.Combine(soupBinFolder, "Monitor.Detours.64.dll")),
				new File(System.IO.Path.Combine(soupBinFolder, "Soup.exe")),
				new File(System.IO.Path.Combine(soupBinFolder, "Soup.Cpp.dll")),
				new File(System.IO.Path.Combine(soupBinFolder, "Soup.CSharp.dll")),
				new File(System.IO.Path.Combine(soupBinFolder, "Soup.Generate.exe"))),
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
		project.OutDir = @"msi";

		// Prevent two installations at the same time
		project.MajorUpgrade = new MajorUpgrade()
		{
			Schedule = UpgradeSchedule.afterInstallInitialize,
			DowngradeErrorMessage = "A later version of [ProductName] is already installed. Setup will now exit.",
		};

		// Upgrade values
		project.Version = new Version(0, 11, 0);

		Compiler.BuildMsi(project);
	}
}