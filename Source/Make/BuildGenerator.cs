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
			var includeItems = new List<string>();
			var sourceItems = new List<string>();
			foreach (var file in PackageManager.FindSourceFiles(recipe, packageDirectory))
			{
				var filePath = $"$(PackageRoot)\\{file}";
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
				await writer.WriteLineAsync("SHELL = /bin/sh");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("PACKAGE_ROOT = ../..");
				await writer.WriteLineAsync("SRCDIR = $(PACKAGE_ROOT)/src");
				await writer.WriteLineAsync("OBJDIR = $(PACKAGE_ROOT)/obj");
				await writer.WriteLineAsync("BINDIR = $(PACKAGE_ROOT)/bin");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("LINK = ar");
				await writer.WriteLineAsync("LFLAGS = rcs");
				await writer.WriteLineAsync("CPP = gcc");
				await writer.WriteLineAsync("CFLAGS = -g -Wall -D SOUP_PKG_ACTIVE=inline -D SOUP_PKG_VERSION=v1_0_0");
				await writer.WriteLineAsync("INCLUDES =");
				await writer.WriteLineAsync("SOURCE = ColorF.cpp");
				await writer.WriteLineAsync("OBJS = obj/ColorF.o");
				await writer.WriteLineAsync("LIB = $(BINDIR)/CppColor.a");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("All: $(LIB)");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(LIB): $(OBJDIR) $(OBJS) $(BINDIR)");
				await writer.WriteLineAsync("	$(LINK) $(LFLAGS) $@ $(OBJS)");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(OBJDIR):");
				await writer.WriteLineAsync("	mkdir $@");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(BINDIR):");
				await writer.WriteLineAsync("	mkdir $@");
				await writer.WriteLineAsync("");

				await writer.WriteLineAsync("$(OBJDIR)/%.o: $(SRCDIR)/%.cpp");
				await writer.WriteLineAsync("	$(CC) $(CFLAGS) -c -o $@ $<");
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
