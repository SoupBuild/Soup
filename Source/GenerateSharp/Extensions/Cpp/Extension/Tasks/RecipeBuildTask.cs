// <copyright file="RecipeBuildTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Cpp.Compiler;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Cpp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	public class RecipeBuildTask : IBuildTask
	{
		private IBuildState buildState;
		private IValueFactory factory;

		/// <summary>
		/// Get the run before list
		/// </summary>
		public static IReadOnlyList<string> RunBeforeList => new List<string>()
		{
			"BuildTask",
		};

		/// <summary>
		/// Get the run after list
		/// </summary>
		public static IReadOnlyList<string> RunAfterList => new List<string>()
		{
			"ResolveToolsTask",
		};

		public RecipeBuildTask(IBuildState buildState, IValueFactory factory)
		{
			this.buildState = buildState;
			this.factory = factory;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			var rootTable = this.buildState.ActiveState;
			var parametersTable = rootTable["Parameters"].AsTable();
			var recipeTable = rootTable["Recipe"].AsTable();
			var buildTable = rootTable.EnsureValueTable(this.factory, "Build");

			// Load the input properties
			var compilerName = parametersTable["Compiler"].AsString();
			var packageRoot = new Path(parametersTable["PackageDirectory"].AsString());
			var buildFlavor = parametersTable["Flavor"].AsString();
			var platformLibraries = rootTable["PlatformLibraries"].AsList().Select(value => new Path(value.AsString())).ToList();
			var platformIncludePaths = rootTable["PlatformIncludePaths"].AsList().Select(value => new Path(value.AsString())).ToList();
			var platformLibraryPaths = rootTable["PlatformLibraryPaths"].AsList().Select(value => new Path(value.AsString())).ToList();
			var platformPreprocessorDefinitions = rootTable["PlatformPreprocessorDefinitions"].AsList().Select(value => value.AsString()).ToList();

			// Load Recipe properties
			var name = recipeTable["Name"].AsString();

			// Add any explicit platform dependencies that were added in the recipe
			if (recipeTable.TryGetValue("PlatformLibraries", out var platformLibrariesValue))
			{
				foreach (var value in platformLibrariesValue.AsList().Select(value => new Path(value.AsString())))
				{
					platformLibraries.Add(value);
				}
			}

			// Add the dependency static library closure to link if targeting an executable or dynamic library
			var linkLibraries = new List<Path>();
			if (recipeTable.TryGetValue("LinkLibraries", out var linkLibrariesValue))
			{
				foreach (var value in linkLibrariesValue.AsList().Select(value => new Path(value.AsString())))
				{
					// If relative then resolve to working directory
					if (value.HasRoot)
					{
						linkLibraries.Add(value);
					}
					else
					{
						linkLibraries.Add(packageRoot + value);
					}
				}
			}

			// Add the dependency runtime dependencies closure if present
			if (recipeTable.TryGetValue("RuntimeDependencies", out var recipeRuntimeDependenciesValue))
			{
				var runtimeDependencies = new List<Path>();
				if (buildTable.TryGetValue("RuntimeDependencies", out var buildRuntimeDependenciesValue))
				{
					runtimeDependencies = buildRuntimeDependenciesValue.AsList().Select(value => new Path(value.AsString())).ToList();
				}

				foreach (var value in recipeRuntimeDependenciesValue.AsList().Select(value => new Path(value.AsString())))
				{
					// If relative then resolve to working directory
					if (value.HasRoot)
					{
						runtimeDependencies.Add(value);
					}
					else
					{
						runtimeDependencies.Add(packageRoot + value);
					}
				}

				buildTable.EnsureValueList(this.factory, "RuntimeDependencies").SetAll(this.factory, runtimeDependencies);
			}

			// Combine the include paths from the recipe and the system
			var includePaths = new List<Path>();
			if (recipeTable.TryGetValue("IncludePaths", out var includePathsValue))
			{
				includePaths = includePathsValue.AsList().Select(value => new Path(value.AsString())).ToList();
			}

			// Add the platform include paths
			includePaths.AddRange(platformIncludePaths);

			// Load the extra library paths provided to the build system
			var libraryPaths = new List<Path>();

			// Add the platform library paths
			libraryPaths.AddRange(platformLibraryPaths);

			// Combine the defines with the default set and the platform
			var preprocessorDefinitions = new List<string>();
			if (recipeTable.TryGetValue("Defines", out var definesValue))
			{
				preprocessorDefinitions = definesValue.AsList().Select(value => value.AsString()).ToList();
			}

			preprocessorDefinitions.AddRange(platformPreprocessorDefinitions);
			preprocessorDefinitions.Add("SOUP_BUILD");

			// Build up arguments to build this individual recipe
			var targetDirectory = new Path(parametersTable["TargetDirectory"].AsString());
			var binaryDirectory = new Path("bin/");
			var objectDirectory = new Path("obj/");

			// Load the resources file if present
			var resourcesFile = string.Empty;
			if (recipeTable.TryGetValue("Resources", out var resourcesValue))
			{
				var resourcesFilePath = new Path(resourcesValue.AsString());

				resourcesFile = resourcesFilePath.ToString();
			}

			// Load the module interface partition files if present
			var moduleInterfacePartitionSourceFiles = new List<IValue>();
			if (recipeTable.TryGetValue("Partitions", out var partitionsValue))
			{
				foreach (var partition in partitionsValue.AsList())
				{
					var targetPartitionTable = this.factory.CreateTable();
					if (partition.IsString())
					{
						targetPartitionTable.Add(
							"Source",
							this.factory.Create(partition.AsString()));
					}
					else if (partition.IsTable())
					{
						var partitionTable = partition.AsTable();
						if (partitionTable.TryGetValue("Source", out var partitionSourceValue))
						{
							targetPartitionTable.Add(
								"Source",
								this.factory.Create(partitionSourceValue.AsString()));
						}
						else
						{
							throw new InvalidOperationException("Partition table missing Source");
						}

						if (partitionTable.TryGetValue("Imports", out var partitionImportsValue))
						{
							var partitionImports = partitionImportsValue.AsList().Select(value => this.factory.Create(value.AsString()));
							targetPartitionTable.Add(
								"Imports",
								this.factory.Create(this.factory.CreateList().Append(partitionImports)));
						}
					}
					else
					{
						throw new InvalidOperationException("Unknown partition type.");
					}

					moduleInterfacePartitionSourceFiles.Add(this.factory.Create(targetPartitionTable));
				}
			}

			// Load the module interface file if present
			var moduleInterfaceSourceFile = string.Empty;
			if (recipeTable.TryGetValue("Interface", out var interfaceValue))
			{
				var moduleInterfaceSourceFilePath = new Path(interfaceValue.AsString());

				// TODO: Clang requires annoying cppm extension
				if (compilerName == "Clang")
				{
					moduleInterfaceSourceFilePath.SetFileExtension("cppm");
				}

				moduleInterfaceSourceFile = moduleInterfaceSourceFilePath.ToString();
			}

			// Load the source files if present
			var sourceFiles = new List<string>();
			if (recipeTable.TryGetValue("Source", out var sourceValue))
			{
				sourceFiles = sourceValue.AsList().Select(value => value.AsString()).ToList();
			}

			// Load the assembly source files if present
			var assemblySourceFiles = new List<string>();
			if (recipeTable.TryGetValue("AssemblySource", out var assemblySourceValue))
			{
				assemblySourceFiles = assemblySourceValue.AsList().Select(value => value.AsString()).ToList();
			}

			// Check for warning settings
			bool enableWarningsAsErrors = true;
			if (recipeTable.TryGetValue("EnableWarningsAsErrors", out var enableWarningsAsErrorsValue))
			{
				enableWarningsAsErrors = enableWarningsAsErrorsValue.AsBoolean();
			}

			// Set the correct optimization level for the requested flavor
			var optimizationLevel = BuildOptimizationLevel.None;
			bool generateSourceDebugInfo = false;
			if (string.Compare(buildFlavor, "debug", StringComparison.OrdinalIgnoreCase) == 0)
			{
				// preprocessorDefinitions.pushthis.back("DEBUG");
				generateSourceDebugInfo = true;
			}
			else if (string.Compare(buildFlavor, "debugrelease", StringComparison.OrdinalIgnoreCase) == 0)
			{
				preprocessorDefinitions.Add("RELEASE");
				generateSourceDebugInfo = true;
				optimizationLevel = BuildOptimizationLevel.Speed;
			}
			else if (string.Compare(buildFlavor, "release", StringComparison.OrdinalIgnoreCase) == 0)
			{
				preprocessorDefinitions.Add("RELEASE");
				optimizationLevel = BuildOptimizationLevel.Speed;
			}
			else
			{
				this.buildState.LogTrace(TraceLevel.Error, "Unknown build flavor type.");
				throw new InvalidOperationException("Unknown build flavors type.");
			}

			buildTable["TargetName"] = this.factory.Create(name);
			buildTable["SourceRootDirectory"] = this.factory.Create(packageRoot.ToString());
			buildTable["TargetRootDirectory"] = this.factory.Create(targetDirectory.ToString());
			buildTable["ObjectDirectory"] = this.factory.Create(objectDirectory.ToString());
			buildTable["BinaryDirectory"] = this.factory.Create(binaryDirectory.ToString());
			buildTable["ResourcesFile"] = this.factory.Create(resourcesFile);
			buildTable.EnsureValueList(this.factory, "ModuleInterfacePartitionSourceFiles").Append(moduleInterfacePartitionSourceFiles);
			buildTable["ModuleInterfaceSourceFile"] = this.factory.Create(moduleInterfaceSourceFile);
			buildTable["OptimizationLevel"] = this.factory.Create((long)optimizationLevel);
			buildTable["GenerateSourceDebugInfo"] = this.factory.Create(generateSourceDebugInfo);

			buildTable.EnsureValueList(this.factory, "PlatformLibraries").Append(this.factory, platformLibraries);
			buildTable.EnsureValueList(this.factory, "LinkLibraries").Append(this.factory, linkLibraries);
			buildTable.EnsureValueList(this.factory, "PreprocessorDefinitions").Append(this.factory, preprocessorDefinitions);
			buildTable.EnsureValueList(this.factory, "IncludeDirectories").Append(this.factory, includePaths);
			buildTable.EnsureValueList(this.factory, "LibraryPaths").Append(this.factory, libraryPaths);
			buildTable.EnsureValueList(this.factory, "Source").Append(this.factory, sourceFiles);
			buildTable.EnsureValueList(this.factory, "AssemblySource").Append(this.factory, assemblySourceFiles);

			buildTable["EnableWarningsAsErrors"] = this.factory.Create(enableWarningsAsErrors);

			// Convert the recipe type to the required build type
			var targetType = BuildTargetType.StaticLibrary;
			if (recipeTable.TryGetValue("Type", out var typeValue))
			{
				targetType = ParseType(typeValue.AsString());
			}

			buildTable["TargetType"] = this.factory.Create((long)targetType);

			// Convert the recipe language version to the required build language
			var languageStandard = LanguageStandard.CPP20;
			if (recipeTable.TryGetValue("LanguageVersion", out var languageVersionValue))
			{
				languageStandard = ParseLanguageStandard(languageVersionValue.AsString());
			}

			buildTable["LanguageStandard"] = this.factory.Create((long)languageStandard);
		}

		private static BuildTargetType ParseType(string value)
		{
			if (value == "Executable")
				return BuildTargetType.Executable;
			else if (value == "Windows")
				return BuildTargetType.WindowsApplication;
			else if (value == "StaticLibrary")
				return BuildTargetType.StaticLibrary;
			else if (value == "DynamicLibrary")
				return BuildTargetType.DynamicLibrary;
			else
				throw new InvalidOperationException("Unknown target type value.");
		}

		private static LanguageStandard ParseLanguageStandard(string value)
		{
			if (value == "C++11")
				return LanguageStandard.CPP11;
			else if (value == "C++14")
				return LanguageStandard.CPP14;
			else if (value == "C++17")
				return LanguageStandard.CPP17;
			else if (value == "C++20")
				return LanguageStandard.CPP20;
			else
				throw new InvalidOperationException("Unknown recipe language standard value.");
		}
	}
}
