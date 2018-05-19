// <copyright company="Soup" file="BuildGenerator.cs">
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
	public class BuildGenerator : IBuildGenerator
	{
		public string Name => "Make";

		public async Task GenerateBuildAsync(
			Recipe recipe,
			string targetDirectory,
			string packageDirectory,
			string binaryDirectory,
			string objectDirectory)
		{
			var versionNamespace = PackageManager.BuildNamespaceVersion(recipe.Version);
			var projectName = recipe.Name;

			var includeItems = new List<string>();
			var sourceItems = new List<string>();
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
				await writer.WriteLineAsync("LINK = ar");
				await writer.WriteLineAsync("LFLAGS = rcs");
				await writer.WriteLineAsync("CPP = gcc");
				await writer.WriteLineAsync("");

				// Initialize our directory structure
				await writer.WriteLineAsync("PACKAGE_ROOT = ../..");
				await writer.WriteLineAsync($"OBJDIR = {objectDirectory}");
				await writer.WriteLineAsync($"BINDIR = {binaryDirectory}");
				await writer.WriteLineAsync("");

				// Build the source list
				await writer.WriteLineAsync("SOURCE = \\");
				foreach (var sourceItem in sourceItems)
				{
					await writer.WriteLineAsync($"\t{sourceItem} \\");
				}

				await writer.WriteLineAsync("");

				await writer.WriteLineAsync($"CFLAGS = -g -Wall -D SOUP_PKG_ACTIVE=inline -D SOUP_PKG_VERSION={versionNamespace}");
				await writer.WriteLineAsync("INCLUDES =");
				await writer.WriteLineAsync("OBJS = $(OBJDIR)/ColorF.o");
				await writer.WriteLineAsync($"LIB = $(BINDIR)/{projectName}.a");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("All: $(LIB)");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(LIB): $(OBJDIR) $(OBJS) $(BINDIR)");
				await writer.WriteLineAsync("	$(LINK) $(LFLAGS) $@ $(OBJS)");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(OBJDIR):");
				await writer.WriteLineAsync("	mkdir -p $@");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(BINDIR):");
				await writer.WriteLineAsync("	mkdir -p $@");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(OBJDIR)/%.o: $(PACKAGE_ROOT)/src/%.cpp");
				await writer.WriteLineAsync("	$(CPP) $(CFLAGS) -c -o $@ $<");
			}
		}

		public async Task GenerateDependenciesAsync(
			Recipe recipe,
			string targetDirectory)
		{
			// Build up the closure of dependecies for include paths and libraries
			var dependencies = new List<string>()
			{
				"$(LIBS)"
			};
			var includes = new List<string>()
			{
				"$(INCLUDE)"
			};

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
			using (var writer = new StreamWriter(File.Create(propertiesFilePath), Encoding.UTF8))
			{
			}
		}
	}
}
