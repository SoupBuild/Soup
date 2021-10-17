// <copyright file="RecipeBuildTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.CSharp.Compiler;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.CSharp
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

			// Load Recipe properties
			var name = recipeTable["Name"].AsString();

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

			// Load the extra library paths provided to the build system
			var libraryPaths = new List<Path>();

			// Combine the defines with the default set and the platform
			var preprocessorDefinitions = new List<string>();
			if (recipeTable.TryGetValue("Defines", out var definesValue))
			{
				preprocessorDefinitions = definesValue.AsList().Select(value => value.AsString()).ToList();
			}

			preprocessorDefinitions.Add("SOUP_BUILD");

			// Build up arguments to build this individual recipe
			var targetDirectory = new Path(parametersTable["TargetDirectory"].AsString());
			var binaryDirectory = targetDirectory + new Path("bin/");
			var objectDirectory = targetDirectory + new Path("obj/");

			// Load the source files if present
			var sourceFiles = new List<string>();
			if (recipeTable.TryGetValue("Source", out var sourceValue))
			{
				sourceFiles = sourceValue.AsList().Select(value => value.AsString()).ToList();
			}

			// Check for warning settings
			bool enableWarningsAsErrors = true;
			if (recipeTable.TryGetValue("EnableWarningsAsErrors", out var enableWarningsAsErrorsValue))
			{
				enableWarningsAsErrors = enableWarningsAsErrorsValue.AsBoolean();
			}

			// Check for nullable settings, default to enabled
			var nullableState = BuildNullableState.Enabled;
			if (recipeTable.TryGetValue("Nullable", out var nullableValue))
			{
				nullableState = ParseNullable(nullableValue.AsString());
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
			buildTable["WorkingDirectory"] = this.factory.Create(packageRoot.ToString());
			buildTable["ObjectDirectory"] = this.factory.Create(objectDirectory.ToString());
			buildTable["BinaryDirectory"] = this.factory.Create(binaryDirectory.ToString());
			buildTable["OptimizationLevel"] = this.factory.Create((long)optimizationLevel);
			buildTable["GenerateSourceDebugInfo"] = this.factory.Create(generateSourceDebugInfo);

			buildTable.EnsureValueList(this.factory, "LinkLibraries").Append(this.factory, linkLibraries);
			buildTable.EnsureValueList(this.factory, "PreprocessorDefinitions").Append(this.factory, preprocessorDefinitions);
			buildTable.EnsureValueList(this.factory, "LibraryPaths").Append(this.factory, libraryPaths);
			buildTable.EnsureValueList(this.factory, "Source").Append(this.factory, sourceFiles);

			buildTable["EnableWarningsAsErrors"] = this.factory.Create(enableWarningsAsErrors);
			buildTable["NullableState"] = this.factory.Create((long)nullableState);

			// Convert the recipe type to the required build type
			var targetType = BuildTargetType.Library;
			if (recipeTable.TryGetValue("Type", out var typeValue))
			{
				targetType = ParseType(typeValue.AsString());
			}

			buildTable["TargetType"] = this.factory.Create((long)targetType);
		}

		private static BuildTargetType ParseType(string value)
		{
			if (value == "Executable")
				return BuildTargetType.Executable;
			else if (value == "Library")
				return BuildTargetType.Library;
			else if (value == "Module")
				return BuildTargetType.Module;
			else
				throw new InvalidOperationException("Unknown target type value.");
		}

		private static BuildNullableState ParseNullable(string value)
		{
			if (value == "Enabled")
				return BuildNullableState.Enabled;
			else if (value == "Disabled")
				return BuildNullableState.Disabled;
			else if (value == "Warnings")
				return BuildNullableState.Warnings;
			else if (value == "Annotations")
				return BuildNullableState.Annotations;
			else
				throw new InvalidOperationException("Unknown nullable state value.");
		}
	}
}
