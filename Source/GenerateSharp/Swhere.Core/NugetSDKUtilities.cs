// <copyright file="NugetSDKUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Swhere.Core.Nuget;
using System;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;

namespace Soup.Build.Discover
{
	internal static class NugetSDKUtilities
	{
		public static async Task FindNugetPackagesAsync()
		{
			var fileSystem = LifetimeManager.Get<IFileSystem>();
			var nugetDirectory = fileSystem.GetUserProfileDirectory() +
				new Path(".nuget");
			var nugetPackagesDirectory = nugetDirectory +
				new Path("packages");
			if (fileSystem.Exists(nugetPackagesDirectory))
			{
				Log.Info($"Discover Nuget Packages: {nugetPackagesDirectory}");
				foreach (var nugetPackageDirectory in fileSystem.GetChildDirectories(nugetPackagesDirectory))
				{
					var packageName = nugetPackageDirectory.Path.GetFileName();
					foreach (var nugetPackageVersionDirectory in fileSystem.GetChildDirectories(nugetPackageDirectory.Path))
					{
						var packageVersion = nugetPackageVersionDirectory.Path.GetFileName();
						await LoadNugetPackageAsync(packageName, packageVersion, nugetPackageVersionDirectory.Path);
					}
				}
			}
			else
			{
				Log.Info("Nuget not found");
			}
		}

		private static async Task LoadNugetPackageAsync(string name, string version, Path directory)
		{
			var fileSystem = LifetimeManager.Get<IFileSystem>();

			// Load the NuSpec XML file
			var nuspecFile = directory + new Path($"{name}.nuspec");
			if (!fileSystem.Exists(nuspecFile))
			{
				Log.Warning($"Missing Nuspec file: {nuspecFile}");
				return;
			}

			using var nuspec = fileSystem.OpenRead(nuspecFile);
			using var reader = XmlReader.Create(nuspec.GetInStream(), new XmlReaderSettings() {  Async = true });
			var rootNamespace = await ReadRootNamespaceAsync(reader);
			if (!Regex.IsMatch(rootNamespace, @"^http://schemas.microsoft.com/packaging/\d\d\d\d/\d\d/nuspec.xsd$"))
			{
				Log.Warning($"Unknown Nuspec file namespace: {rootNamespace}");
				return;
			}

			var serializer = new XmlSerializer(typeof(NuspecPackage), rootNamespace);

			if (!serializer.CanDeserialize(reader))
			{
				Log.Warning($"OLD Nuspec file: {nuspecFile}");
				return;
			}

			var nuspecDocument = (NuspecPackage?)serializer.Deserialize(reader);

			await Task.CompletedTask;
		}

		private static async Task<string> ReadRootNamespaceAsync(XmlReader reader)
		{
			while (await reader.ReadAsync())
			{
				switch (reader.NodeType)
				{
					case XmlNodeType.Element:
						return reader.NamespaceURI;
				}
			}

			throw new InvalidOperationException("XML had no root");
		}
	}
}