using System;
using WixSharp;

class Script
{
	static public void Main()
	{
		var soupRunFolder = @"../../../out/run";
		var project = new Project(
			"Soup Build",
			new Dir(
				@"%ProgramFiles%\SoupBuild\Soup",
				new DirFiles($"{soupRunFolder}/*.*"),
				new Dir(
					@"Soup",
					new DirFiles($"{soupRunFolder}/Soup/*.*"),
					new Dir(
						@"Generate",
						new DirFiles($"{soupRunFolder}/Soup/Generate/*.*"),
						new Dir(
							@"Extensions",
							new Dir(
								@"Soup.Cpp",
								new DirFiles($"{soupRunFolder}/Soup/Generate/Extensions/Soup.Cpp/*.*")),
							new Dir(
								@"Soup.CSharp",
								new DirFiles($"{soupRunFolder}/Soup/Generate/Extensions/Soup.CSharp/*.*")))),
					new Dir(
						@"PackageManager",
						new DirFiles($"{soupRunFolder}/Soup/PackageManager/*.*"),
						new Dir(
							@"runtimes",
							new Dir(
								@"win-arm64\native",
								new DirFiles($"{soupRunFolder}/Soup/PackageManager/runtimes/win-arm64/native/*.*")),
							new Dir(
								@"win-x64\native",
								new DirFiles($"{soupRunFolder}/Soup/PackageManager/runtimes/win-x64/native/*.*")),
							new Dir(
								@"win-x86\native",
								new DirFiles($"{soupRunFolder}/Soup/PackageManager/runtimes/win-x86/native/*.*"))))),
				new Dir(
					@"Swhere",
					new DirFiles($"{soupRunFolder}/Swhere/*.*"))),
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
		project.OutFileName = "SoupBuild";

		// Prevent two installations at the same time
		project.MajorUpgrade = new MajorUpgrade()
		{
			Schedule = UpgradeSchedule.afterInstallInitialize,
			DowngradeErrorMessage = "A later version of [ProductName] is already installed. Setup will now exit.",
		};

		// Upgrade values
		project.Version = new Version(0, 17, 4);

		Compiler.BuildMsi(project);
	}
}