// <copyright company="Soup" file="MakeBuildGenerator.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Make
{
	using System;
	using System.Collections.Generic;
	using System.IO;
	using System.Text;
	using System.Threading.Tasks;
	using System.Xml.Serialization;

	/// <summary>
	/// The Make generator
	/// </summary>
	internal static class MakeBuildGenerator
	{
		public static Task GenerateBuildAsync(
			Recipe recipe,
			string targetDirectory,
			string buildDirectory,
			string packageDirectory,
			string binaryDirectory,
			string objectDirectory)
		{
			switch (recipe.Type)
			{
				case RecipeType.Executable:
					return GenerateExecutableBuildAsync(
						recipe, 
						targetDirectory,
						buildDirectory,
						packageDirectory,
						binaryDirectory,
						objectDirectory);
				case RecipeType.Library:
					return GenerateLibraryBuildAsync(
						recipe, 
						targetDirectory,
						buildDirectory,
						packageDirectory,
						binaryDirectory,
						objectDirectory);
				default:
					throw new NotSupportedException($"Unknown recipe type {recipe.Type}");
			}
		}

		public static async Task GenerateDependenciesAsync(
			Recipe recipe,
			string targetDirectory)
		{
			// Build up the closure of dependecies for include paths and libraries
			var dependencies = new List<string>();
			var includes = new List<string>();

			var dependencyClosure = await PackageManager.BuildRecursiveDependeciesAsync(recipe);
			foreach (var dependency in dependencyClosure)
			{
				var libraryName = string.Format(
					Constants.LibraryTargetNameFormat,
					dependency.Name,
					dependency.Version,
					"$(Platform)",
					"$(Configuration)");
				dependencies.Add($"{libraryName}.lib");

				var includePath = PackageManager.BuildKitchenIncludePath(dependency);
				includes.Add(includePath);
			}

			var propertiesFilePath = Path.Combine(targetDirectory, MSBuildConstants.MakeIncFileName);
			Log.Message(targetDirectory);
			using (var writer = new StreamWriter(File.Create(propertiesFilePath), Encoding.UTF8))
			{
				await writer.WriteLineAsync("");
				await writer.WriteLineAsync("INCLUDES = \\");
				foreach (var include in includes)
				{
					await writer.WriteLineAsync($"\t{include} \\");
				}
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("LIBS = \\");
				foreach (var library in dependencies)
				{
					await writer.WriteLineAsync($"\t{library} \\");
				}
				await writer.WriteLineAsync("");
			}
		}

		private static async Task GenerateExecutableBuildAsync(
			Recipe recipe,
			string targetDirectory,
			string buildDirectory,
			string packageDirectory,
			string binaryDirectory,
			string objectDirectory)
		{
			var versionNamespace = PackageManager.BuildNamespaceVersion(recipe.Version);
			var projectName = recipe.Name;
			var relativePackageRoot = Path.GetRelativePath(
				buildDirectory.EnsureTrailingSlash(),
				packageDirectory.EnsureTrailingSlash()).RemoveTrailingSlash();
			var relativeBinaryRoot = Path.GetRelativePath(
				buildDirectory.EnsureTrailingSlash(),
				binaryDirectory.EnsureTrailingSlash()).RemoveTrailingSlash();
			var relativeObjectRoot = Path.GetRelativePath(
				buildDirectory.EnsureTrailingSlash(),
				objectDirectory.EnsureTrailingSlash()).RemoveTrailingSlash();

			var includeItems = new List<string>();
			var sourceItems = new List<string>();
			var sourceFolderSet = new HashSet<string>();
			foreach (var file in PackageManager.FindSourceFiles(recipe, packageDirectory))
			{
				var filePath = $"{file}";
				switch (Path.GetExtension(file))
				{
					case ".h":
						includeItems.Add(filePath);
						break;
					case ".cpp":
						sourceItems.Add(filePath);
						sourceFolderSet.Add(Path.GetDirectoryName(filePath));
						break;
					default:
						throw new InvalidOperationException("A source file with unknown extension.");
				}
			}

			var projectFileName = MSBuildConstants.MakeFileName;
			var projectFilePath = Path.Combine(targetDirectory, projectFileName);
			using (var writer = new StreamWriter(File.Create(projectFilePath), Encoding.UTF8))
			{
				// Include the shared make include
				await writer.WriteLineAsync("include Make.inc");
				await writer.WriteLineAsync("");

				// Set the shell
				await writer.WriteLineAsync("SHELL = /bin/sh");
				await writer.WriteLineAsync("");

				// Setup the tools used in our builds
				await writer.WriteLineAsync("CXX = g++");
				await writer.WriteLineAsync("");

				// Initialize our directory structure
				await writer.WriteLineAsync($"PACKAGE_ROOT = {relativePackageRoot}");
				await writer.WriteLineAsync($"OBJDIR = {relativeObjectRoot}");
				await writer.WriteLineAsync($"BINDIR = {relativeBinaryRoot}");
				await writer.WriteLineAsync("DIRS = \\");
				await writer.WriteLineAsync("	$(OBJDIR) \\");
				foreach (var sourceFolder in sourceFolderSet)
				{
					await writer.WriteLineAsync($"	$(OBJDIR)/{sourceFolder} \\");
				}

				await writer.WriteLineAsync("	$(BINDIR) \\");
				await writer.WriteLineAsync("");

				// Build the source list
				await writer.WriteLineAsync($"EXE = $(BINDIR)/{projectName}");
				await writer.WriteLineAsync("SOURCE = \\");
				foreach (var sourceItem in sourceItems)
				{
					await writer.WriteLineAsync($"\t{sourceItem} \\");
				}
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("OBJS = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(SOURCE))))");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync($"SOUPDEFINES = -D SOUP_PKG_ACTIVE=inline -D SOUP_PKG_VERSION={versionNamespace}");
				await writer.WriteLineAsync($"CPPFLAGS = -g -Wall $(addprefix -I, $(INCLUDES)) $(SOUPDEFINES)");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("All: $(DIRS) $(EXE)");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(EXE): $(OBJS)");
				await writer.WriteLineAsync("	$(CXX) $(CPPFLAGS) $< -o $@");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(DIRS):");
				await writer.WriteLineAsync("	mkdir -p $@");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(OBJDIR)/%.o: $(PACKAGE_ROOT)/%.cpp");
				foreach (var sourceFolder in sourceFolderSet)
				{
					await writer.WriteLineAsync($"$(OBJDIR)/{sourceFolder}/%.o: $(PACKAGE_ROOT)/{sourceFolder}/%.cpp");
				}
				await writer.WriteLineAsync("	$(CXX) -c $(CPPFLAGS) $< -o $@");
			}
		}

		private static async Task GenerateLibraryBuildAsync(
			Recipe recipe,
			string targetDirectory,
			string buildDirectory,
			string packageDirectory,
			string binaryDirectory,
			string objectDirectory)
		{
			var versionNamespace = PackageManager.BuildNamespaceVersion(recipe.Version);
			var projectName = recipe.Name;
			var relativePackageRoot = Path.GetRelativePath(
				buildDirectory.EnsureTrailingSlash(),
				packageDirectory.EnsureTrailingSlash()).RemoveTrailingSlash();
			var relativeBinaryRoot = Path.GetRelativePath(
				buildDirectory.EnsureTrailingSlash(),
				binaryDirectory.EnsureTrailingSlash()).RemoveTrailingSlash();
			var relativeObjectRoot = Path.GetRelativePath(
				buildDirectory.EnsureTrailingSlash(),
				objectDirectory.EnsureTrailingSlash()).RemoveTrailingSlash();
					
			var includeItems = new List<string>();
			var sourceItems = new List<string>();
			var sourceFolderSet = new HashSet<string>();
			foreach (var file in PackageManager.FindSourceFiles(recipe, packageDirectory))
			{
				var filePath = $"{file}";
				switch (Path.GetExtension(file))
				{
					case ".h":
						includeItems.Add(filePath);
						break;
					case ".cpp":
						sourceItems.Add(filePath);
						sourceFolderSet.Add(Path.GetDirectoryName(filePath));
						break;
					default:
						throw new InvalidOperationException("A source file with unknown extension.");
				}
			}

			var projectFileName = MSBuildConstants.MakeFileName;
			var projectFilePath = Path.Combine(targetDirectory, projectFileName);
			using (var writer = new StreamWriter(File.Create(projectFilePath), Encoding.UTF8))
			{
				// Set the shell
				await writer.WriteLineAsync("SHELL = /bin/sh");
				await writer.WriteLineAsync("");

				// Setup the tools used in our builds
				await writer.WriteLineAsync("AR = ar");
				await writer.WriteLineAsync("LFLAGS = rcs");
				await writer.WriteLineAsync("CXX = gcc");
				await writer.WriteLineAsync("");

				// Initialize our directory structure
				await writer.WriteLineAsync($"PACKAGE_ROOT = {relativePackageRoot}");
				await writer.WriteLineAsync($"OBJDIR = {relativeObjectRoot}");
				await writer.WriteLineAsync($"BINDIR = {relativeBinaryRoot}");
				await writer.WriteLineAsync("DIRS = \\");
				await writer.WriteLineAsync("	$(OBJDIR) \\");
				foreach (var sourceFolder in sourceFolderSet)
				{
					await writer.WriteLineAsync($"	$(OBJDIR)/{sourceFolder} \\");
				}

				await writer.WriteLineAsync("	$(BINDIR) \\");
				await writer.WriteLineAsync("");

				// Build the source list
				await writer.WriteLineAsync($"LIB = $(BINDIR)/{projectName}.a");
				await writer.WriteLineAsync("SOURCE = \\");
				foreach (var sourceItem in sourceItems)
				{
					await writer.WriteLineAsync($"\t{sourceItem} \\");
				}
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("OBJS = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(SOURCE))))");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync($"CPPFLAGS = -g -Wall -D SOUP_PKG_ACTIVE=inline -D SOUP_PKG_VERSION={versionNamespace}");
				await writer.WriteLineAsync("INCLUDES =");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("All: $(DIRS) $(LIB)");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(LIB): $(OBJS)");
				await writer.WriteLineAsync("	$(AR) $(LFLAGS) $@ $(OBJS)");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(DIRS):");
				await writer.WriteLineAsync("	mkdir -p $@");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(OBJDIR)/%.o: $(PACKAGE_ROOT)/%.cpp");
				foreach (var sourceFolder in sourceFolderSet)
				{
					await writer.WriteLineAsync($"$(OBJDIR)/{sourceFolder}/%.o: $(PACKAGE_ROOT)/{sourceFolder}/%.cpp");
				}
				await writer.WriteLineAsync("	$(CXX) -c $(CPPFLAGS) $< -o $@");
			}
		}
	}
}
