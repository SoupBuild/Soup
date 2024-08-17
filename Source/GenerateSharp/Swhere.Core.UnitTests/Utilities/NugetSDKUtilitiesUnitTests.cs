// <copyright file="DotNetSDKUtilitiesUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Swhere.Core.Nuget;
using System.Text;
using Xunit;
using Path = Opal.Path;

namespace Soup.Build.Discover.UnitTests;

[Collection("Opal")]
public class NugetSDKUtilitiesUnitTests
{
	[Fact]
	public void FindNugetPackages_NotFound()
	{
		// Register the test listener
		var testListener = new TestTraceListener();
		using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

		var mockFileSystem = new MockFileSystem();
		using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

		var result = NugetSDKUtilities.FindNugetPackages();

		Assert.False(result.HasNuget);
		Assert.Equal(
			new Path("C:/Users/Me/.nuget/packages"),
			result.NugetPackagesPath);
		Assert.Equal(
			[],
			result.Packages);

		// Verify expected logs
		Assert.Equal(
			[
				"INFO: Nuget not found",
			],
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			[
				"GetUserProfileDirectory",
				"Exists: C:/Users/Me/.nuget/packages",
			],
			mockFileSystem.Requests);
	}

	[Fact]
	public void FindNugetPackages_MissingNupac()
	{
		// Register the test listener
		var testListener = new TestTraceListener();
		using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

		var mockFileSystem = new MockFileSystem();
		using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

		mockFileSystem.RegisterChildren(
			new Path("C:/Users/Me/.nuget/packages"),
			[
				new DirectoryEntry() { Path = new Path("C:/Users/Me/.nuget/packages/Package1"), IsDirectory = true, },
			]);

		mockFileSystem.RegisterChildren(
			new Path("C:/Users/Me/.nuget/packages/Package1"),
			[
				new DirectoryEntry() { Path = new Path("C:/Users/Me/.nuget/packages/Package1/1.2.3"), IsDirectory = true, },
			]);

		var result = NugetSDKUtilities.FindNugetPackages();

		Assert.True(result.HasNuget);
		Assert.Equal(
			new Path("C:/Users/Me/.nuget/packages"),
			result.NugetPackagesPath);
		Assert.Equal(
			[],
			result.Packages);

		// Verify expected logs
		Assert.Equal(
			[
				"INFO: Discover Nuget Packages: C:/Users/Me/.nuget/packages",
				"WARN: Missing Nuspec file: C:/Users/Me/.nuget/packages/Package1/1.2.3/Package1.nuspec",
			],
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			[
				"GetUserProfileDirectory",
				"Exists: C:/Users/Me/.nuget/packages",
				"GetChildDirectories: C:/Users/Me/.nuget/packages",
				"GetChildDirectories: C:/Users/Me/.nuget/packages/Package1",
				"Exists: C:/Users/Me/.nuget/packages/Package1/1.2.3/Package1.nuspec",
			],
			mockFileSystem.Requests);
	}

	[Fact]
	public void FindNugetPackages_Success()
	{
		// Register the test listener
		var testListener = new TestTraceListener();
		using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

		var mockFileSystem = new MockFileSystem();
		using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

		mockFileSystem.RegisterChildren(
			new Path("C:/Users/Me/.nuget/packages"),
			[
				new DirectoryEntry() { Path = new Path("C:/Users/Me/.nuget/packages/Package1"), IsDirectory = true, },
			]);

		mockFileSystem.RegisterChildren(
			new Path("C:/Users/Me/.nuget/packages/Package1"),
			[
				new DirectoryEntry() { Path = new Path("C:/Users/Me/.nuget/packages/Package1/1.2.3"), IsDirectory = true, },
			]);

		var nuspecContent =
@"<?xml version=""1.0"" encoding=""utf-8""?>
<package xmlns=""http://schemas.microsoft.com/packaging/2013/05/nuspec.xsd"">
  <metadata>
    <id>Package1</id>
    <version>1.2.3</version>
    <authors>Soup</authors>
    <requireLicenseAcceptance>true</requireLicenseAcceptance>
    <license type=""expression"">MIT</license>
    <licenseUrl>https://licenses.nuget.org/MIT</licenseUrl>
    <icon>cool.png</icon>
    <readme>README.md</readme>
    <projectUrl>https://github.com/Package1/README.md</projectUrl>
    <description>This is the implementation of the package</description>
    <releaseNotes>https://github.com/Package1/CHANGELOG.md</releaseNotes>
    <copyright>©Test Corporation. All rights reserved.</copyright>
    <tags>Test Package</tags>
    <repository type=""git"" url=""https://github.com/Package1"" commit=""1234"" />
    <dependencies>
      <group targetFramework="".NETFramework4.6.1"">
      </group>
      <group targetFramework="".NETCoreApp2.1"">
      </group>
      <group targetFramework=""net5.0"">
      </group>
      <group targetFramework=""net6.0"">
      </group>
      <group targetFramework="".NETStandard2.0"">
      </group>
    </dependencies>
  </metadata>
</package>";

		mockFileSystem.CreateMockFile(
			new Path("C:/Users/Me/.nuget/packages/Package1/1.2.3/Package1.nuspec"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(nuspecContent))));

		var result = NugetSDKUtilities.FindNugetPackages();

		Assert.True(result.HasNuget);
		Assert.Equal(
			new Path("C:/Users/Me/.nuget/packages"),
			result.NugetPackagesPath);
		var expectedPackages = new List<NugetPackage>()
			{
				new NugetPackage()
				{
					Id = "Package1",
					Versions =
					[
						new NugetPackageVersion()
						{
							Version = "1.2.3",
							TargetFrameworks =
							[
								new NugetPackageTargetFramework()
								{
									Name = "net461",
									Libraries = [],
									Dependencies = [],
								},
								new NugetPackageTargetFramework()
								{
									Name = "netcoreapp2.1",
									Libraries = [],
									Dependencies = [],
								},
								new NugetPackageTargetFramework()
								{
									Name = "net5.0",
									Libraries = [],
									Dependencies = [],
								},
								new NugetPackageTargetFramework()
								{
									Name = "net6.0",
									Libraries = [],
									Dependencies = [],
								},
								new NugetPackageTargetFramework()
								{
									Name = "netstandard2.0",
									Libraries = [],
									Dependencies = [],
								},
							],
						},
					],
				},
			};

		Assert.Equivalent(expectedPackages, result.Packages);

		// Verify expected logs
		Assert.Equal(
			[
				"INFO: Discover Nuget Packages: C:/Users/Me/.nuget/packages",
			],
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			[
				"GetUserProfileDirectory",
				"Exists: C:/Users/Me/.nuget/packages",
				"GetChildDirectories: C:/Users/Me/.nuget/packages",
				"GetChildDirectories: C:/Users/Me/.nuget/packages/Package1",
				"Exists: C:/Users/Me/.nuget/packages/Package1/1.2.3/Package1.nuspec",
				"OpenRead: C:/Users/Me/.nuget/packages/Package1/1.2.3/Package1.nuspec",
				"Exists: C:/Users/Me/.nuget/packages/Package1/1.2.3/lib/net461/",
				"Exists: C:/Users/Me/.nuget/packages/Package1/1.2.3/lib/netcoreapp2.1/",
				"Exists: C:/Users/Me/.nuget/packages/Package1/1.2.3/lib/net5.0/",
				"Exists: C:/Users/Me/.nuget/packages/Package1/1.2.3/lib/net6.0/",
				"Exists: C:/Users/Me/.nuget/packages/Package1/1.2.3/lib/netstandard2.0/",
			],
			mockFileSystem.Requests);
	}
}
