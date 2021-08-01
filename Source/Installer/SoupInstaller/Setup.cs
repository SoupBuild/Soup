using System;
using WixSharp;

class Script
{
	static public void Main()
	{
		var soupBinFolder = @"..\..\..\out\C++\Soup\fa11c194b8df931b2241afc7196c809fb2cc5692fd7e38a0ae2c176e63fd7ceb\bin\";
		var soupBinGenerateFolder = @"..\..\..\Source\GenerateSharp\Generate\bin\Release\net5.0\";
		var project = new Project(
			"Soup",
			new Dir(
				@"%ProgramFiles%\SoupBuild\Soup",
				new DirFiles(System.IO.Path.Combine(soupBinFolder, "*.*")),
				new Dir(
					@"Generate",
					new DirFiles(System.IO.Path.Combine(soupBinGenerateFolder, "*.*")),
					new Dir(
						@"Extensions",
						new Dir(
							@"Soup.Cpp",
							new DirFiles(System.IO.Path.Combine(soupBinGenerateFolder, @"Extensions\Soup.Cpp\", "*.*"))),
						new Dir(
							@"Soup.CSharp",
							new DirFiles(System.IO.Path.Combine(soupBinGenerateFolder, @"Extensions\Soup.CSharp\", "*.*")))))),
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
		project.Version = new Version(0, 12, 4);

		Compiler.BuildMsi(project);
	}
}