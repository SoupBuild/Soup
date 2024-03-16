// <copyright file="NugetSDKUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Swhere.Core.Nuget;
using System.Xml;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public static class NugetSDKUtilities
{
	public static (bool HasNuget, Path NugetPackagesPath, IList<NugetPackage> Packages) FindNugetPackages()
	{
		var fileSystem = LifetimeManager.Get<IFileSystem>();
		var nugetDirectory = fileSystem.GetUserProfileDirectory() +
			new Path(".nuget");
		var nugetPackagesDirectory = nugetDirectory +
			new Path("packages");
		if (fileSystem.Exists(nugetPackagesDirectory))
		{
			Log.Info($"Discover Nuget Packages: {nugetPackagesDirectory}");
			var packages = new List<NugetPackage>();
			foreach (var nugetPackageDirectory in fileSystem.GetChildDirectories(nugetPackagesDirectory))
			{
				var packageName = nugetPackageDirectory.Path.FileName;
				NugetPackage? package = null;
				foreach (var nugetPackageVersionDirectory in fileSystem.GetChildDirectories(nugetPackageDirectory.Path))
				{
					var (currentPackage, packageVersion) = LoadNugetPackage(
						packageName,
						nugetPackageVersionDirectory.Path);
					if (currentPackage is not null && packageVersion is not null)
					{
						package ??= currentPackage;

						// TODO: Check that they are compatible?
						package.Versions.Add(packageVersion);
					}
				}

				if (package is not null)
				{
					packages.Add(package);
				}
			}

			return (true, nugetPackagesDirectory, packages);
		}
		else
		{
			Log.Info("Nuget not found");
			return (false, nugetPackagesDirectory, new List<NugetPackage>());
		}
	}

	private static (NugetPackage?, NugetPackageVersion?) LoadNugetPackage(
		string name,
		Path directory)
	{
		var fileSystem = LifetimeManager.Get<IFileSystem>();

		// Load the NuSpec XML file
		var nuspecFile = directory + new Path($"{name}.nuspec");
		if (!fileSystem.Exists(nuspecFile))
		{
			Log.Warning($"Missing Nuspec file: {nuspecFile}");
			return (null, null);
		}

		using var nuspec = fileSystem.OpenRead(nuspecFile);
		using var reader = XmlReader.Create(nuspec.GetInStream(), new XmlReaderSettings() { Async = true });

		NuspecPackage nuspecDocument;
		try
		{
			nuspecDocument = NuspecPackage.Deserialize(reader);
		}
		catch (InvalidOperationException)
		{
			Log.Warning($"Failed to parse Nuspec file: {nuspecFile}");
			return (null, null);
		}

		var nugetPackage = new NugetPackage()
		{
			Id = nuspecDocument.Metadata.Id,
		};
		var nugetPackageVersion = new NugetPackageVersion()
		{
			Version = nuspecDocument.Metadata.Version,
		};

		if (nuspecDocument.Metadata.Dependencies is not null)
		{
			foreach (var dependency in nuspecDocument.Metadata.Dependencies)
			{
				if (dependency is NuspecDependencyGroup dependencyGroup)
				{
					var tfm = GetTFM(dependencyGroup.TargetFramework);
					if (tfm is not null)
					{
						var targetFramework = new NugetPackageTargetFramework()
						{
							Name = tfm,
							Libraries = DiscoverLibraries(directory, tfm),
							Dependencies = dependencyGroup.Dependencies.Select(value =>
								new NugetPackageDependency() { Id = value.Id, Version = value.Version }).ToList(),
						};

						nugetPackageVersion.TargetFrameworks.Add(targetFramework);
					}
				}
				else
				{
					if (nuspecDocument is null)
					{
						Log.Warning($"Cannot parse depedencies not in groupd yet: {nuspecFile}");
						return (null, null);
					}
				}
			}
		}

		return (nugetPackage, nugetPackageVersion);
	}

	private static string? GetTFM(string targetFramework)
	{
		string? targetFrameworkMoniker;
		if (targetFramework.StartsWith("net", StringComparison.InvariantCulture))
		{
			targetFrameworkMoniker = targetFramework;
		}
		else
		{
			targetFrameworkMoniker = targetFramework switch
			{
				".NETCoreApp1.0" => "netcoreapp1.0",
				".NETCoreApp1.1" => "netcoreapp1.1",
				".NETCoreApp2.0" => "netcoreapp2.0",
				".NETCoreApp2.1" => "netcoreapp2.1",
				".NETCoreApp2.2" => "netcoreapp2.2",
				".NETCoreApp3.0" => "netcoreapp3.0",
				".NETCoreApp3.1" => "netcoreapp3.1",
				".NETFramework4.5" => "net45",
				".NETFramework4.5.2" => "net452",
				".NETFramework4.6" => "net46",
				".NETFramework4.6.1" => "net461",
				".NETFramework4.6.2" => "net462",
				".NETStandard1.0" => "netstandard1.0",
				".NETStandard1.1" => "netstandard1.1",
				".NETStandard1.2" => "netstandard1.2",
				".NETStandard1.3" => "netstandard1.3",
				".NETStandard1.4" => "netstandard1.4",
				".NETStandard1.5" => "netstandard1.5",
				".NETStandard1.6" => "netstandard1.4",
				".NETStandard2.0" => "netstandard2.0",
				".NETStandard2.1" => "netstandard2.1",
				_ => null
			};
		}

		return targetFrameworkMoniker;
	}

	private static List<string> DiscoverLibraries(Path directory, string targetFrameworkMoniker)
	{
		var fileSystem = LifetimeManager.Get<IFileSystem>();

		var libraries = new List<string>();
		var targetLibrariesPath = directory + new Path($"lib/{targetFrameworkMoniker}/");
		if (fileSystem.Exists(targetLibrariesPath))
		{
			foreach (var file in fileSystem.GetChildFiles(targetLibrariesPath))
			{
				if (file.Path.FileExtension == ".dll")
				{
					var relativeFile = file.Path.GetRelativeTo(directory);
					libraries.Add(relativeFile.ToString());
				}
			}
		}

		return libraries;
	}
}