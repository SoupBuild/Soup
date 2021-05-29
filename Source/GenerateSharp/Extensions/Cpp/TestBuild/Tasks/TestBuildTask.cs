
using Opal;
using Soup.Build.Cpp;
using Soup.Build.Cpp.Compiler;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Test
{
	/// <summary>
	/// The simple build task that will run after the main build task
	/// </summary>
	public class TestBuildTask : IBuildTask
	{
		private IBuildState _buildState;
		private IDictionary<string, Func<IValueTable, ICompiler>> _compilerFactory;

		/// <summary>
		/// Get the run before list
		/// </summary>
		public static IReadOnlyList<string> RunBeforeList => new List<string>()
		{
		};

		/// <summary>
		/// Get the run after list
		/// </summary>
		public static IReadOnlyList<string> RunAfterList => new List<string>()
		{
			"Build",
		};

		public TestBuildTask(IBuildState buildState) : this(buildState, new Dictionary<string, Func<IValueTable, ICompiler>>())
		{
			// Register default compilers
			_compilerFactory.Add("MSVC", (IValueTable activeState) =>
			{
				var clToolPath = new Path(activeState["MSVC.ClToolPath"].AsString());
				var linkToolPath = new Path(activeState["MSVC.LinkToolPath"].AsString());
				var libToolPath = new Path(activeState["MSVC.LibToolPath"].AsString());
				return new Cpp.Compiler.MSVC.Compiler(clToolPath, linkToolPath, libToolPath);
			});
		}

		public TestBuildTask(IBuildState buildState, Dictionary<string, Func<IValueTable, ICompiler>> compilerFactory)
		{
			_buildState = buildState;
			_compilerFactory = compilerFactory;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			var activeState = _buildState.ActiveState;
			var sharedState = _buildState.SharedState;
			var recipeTable = activeState["Recipe"].AsTable();

			var arguments = new BuildArguments();

			// Load up the common build properties from the original Build table in the active state
			var parametersTable = activeState["Parameters"].AsTable();
			var buildTable = activeState["Build"].AsTable();
			LoadBuildProperties(parametersTable, buildTable, arguments);

			// Load the test properties
			if (recipeTable.TryGetValue("Tests", out var testTableValue))
			{
				LoadTestBuildProperties(testTableValue.AsTable(), arguments);
			}
			else
			{
				throw new InvalidOperationException("No Tests Specified");
			}

			// Initialize the compiler to use
			var compilerName = parametersTable["Compiler"].AsString();
			if (!_compilerFactory.TryGetValue(compilerName, out var compileFactory))
			{
				_buildState.LogTrace(TraceLevel.Error, "Unknown compiler: " + compilerName);
				throw new InvalidOperationException();
			}

			var compiler = compileFactory(activeState);

			var buildEngine = new BuildEngine(compiler);
			var buildResult = buildEngine.Execute(_buildState, arguments);

			// Register the build operations
			foreach (var operation in buildResult.BuildOperations)
			{
				_buildState.CreateOperation(operation);
			}

			_buildState.LogTrace(TraceLevel.Information, "Test Build Generate Done");
		}

		private void LoadBuildProperties(
			IValueTable parametersTable,
			IValueTable buildTable,
			BuildArguments arguments)
		{
			arguments.TargetArchitecture = parametersTable["Architecture"].AsString();
			arguments.LanguageStandard = (LanguageStandard)
				buildTable["LanguageStandard"].AsInteger();
			arguments.WorkingDirectory = new Path(buildTable["WorkingDirectory"].AsString());
			arguments.ObjectDirectory = new Path(buildTable["ObjectDirectory"].AsString());
			arguments.BinaryDirectory = new Path(buildTable["BinaryDirectory"].AsString());

			if (buildTable.TryGetValue("IncludeDirectories", out var includeDirectoriesValue))
			{
				arguments.IncludeDirectories = includeDirectoriesValue.AsList().Select(value => new Path(value.AsString())).ToList();
			}

			if (buildTable.TryGetValue("PlatformLibraries", out var platformLibrariesValue))
			{
				arguments.PlatformLinkDependencies = platformLibrariesValue.AsList().Select(value => new Path(value.AsString())).ToList();
			}

			if (buildTable.TryGetValue("LinkLibraries", out var linkLibrariesValue))
			{
				arguments.LinkDependencies = linkLibrariesValue.AsList().Select(value => new Path(value.AsString())).ToList();
			}

			if (buildTable.TryGetValue("LibraryPaths", out var libraryPathsValue))
			{
				arguments.LibraryPaths = libraryPathsValue.AsList().Select(value => new Path(value.AsString())).ToList();
			}

			if (buildTable.TryGetValue("PreprocessorDefinitions", out var preprocessorDefinitionsValue))
			{
				arguments.PreprocessorDefinitions = preprocessorDefinitionsValue.AsList().Select(value => value.AsString()).ToList();
			}

			if (buildTable.TryGetValue("OptimizationLevel", out var optimizationLevelValue))
			{
				arguments.OptimizationLevel = (BuildOptimizationLevel)
					optimizationLevelValue.AsInteger();
			}
			else
			{
				arguments.OptimizationLevel = BuildOptimizationLevel.None;
			}

			if (buildTable.TryGetValue("GenerateSourceDebugInfo", out var generateSourceDebugInfoValue))
			{
				arguments.GenerateSourceDebugInfo = generateSourceDebugInfoValue.AsBoolean();
			}
			else
			{
				arguments.GenerateSourceDebugInfo = false;
			}

			// Load the runtime dependencies
			if (buildTable.TryGetValue("RuntimeDependencies", out var runtimeDependenciesValue))
			{
				arguments.RuntimeDependencies = MakeUnique(
					runtimeDependenciesValue.AsList().Select(value => new Path(value.AsString())));
			}

			// Load the link dependencies
			if (buildTable.TryGetValue("LinkDependencies", out var linkDependenciesValue))
			{
				arguments.LinkDependencies = MakeUnique(
					linkDependenciesValue.AsList().Select(value => new Path(value.AsString())));
			}

			// Load the module references
			if (buildTable.TryGetValue("ModuleDependencies", out var moduleDependenciesValue))
			{
				arguments.ModuleDependencies = MakeUnique(
					moduleDependenciesValue.AsList().Select(value => new Path(value.AsString())));
			}
		}

		private void LoadTestBuildProperties(
			IValueTable testTable,
			BuildArguments arguments)
		{
			if (testTable.TryGetValue("Source", out var sourceValue))
			{
				arguments.SourceFiles = sourceValue.AsList().Select(value => new Path(value.AsString())).ToList();
			}
			else
			{
				throw new InvalidOperationException("No Test Source Files");
			}

			// Combine the include paths from the recipe and the system
			if (testTable.TryGetValue("IncludePaths", out var includePathsValue))
			{
				arguments.IncludeDirectories = CombineUnique(
					arguments.IncludeDirectories,
					includePathsValue.AsList().Select(value => new Path(value.AsString())));
			}

			if (testTable.TryGetValue("PlatformLibraries", out var platformLibrariesValue))
			{
				arguments.PlatformLinkDependencies = CombineUnique(
					arguments.PlatformLinkDependencies,
					platformLibrariesValue.AsList().Select(value => new Path(value.AsString())));
			}

			arguments.TargetName = "TestHarness";
			arguments.TargetType = BuildTargetType.Executable;
		}

		private void LoadTestDependencyBuildInput(
			IBuildState buildState,
			IValueTable activeState,
			BuildArguments arguments)
		{
			if (activeState.TryGetValue("Dependencies", out var dependenciesValue))
			{
				var dependenciesTable = dependenciesValue.AsTable();
				if (dependenciesTable.TryGetValue("Runtime", out var runtimeValue))
				{
					var runtimeDependenciesTable = runtimeValue.AsTable();
					var buildTable = activeState.EnsureValueTable("Build");

					foreach (var dependencyName in runtimeDependenciesTable.Keys)
					{
						// Combine the core dependency build inputs for the core build task
						_buildState.LogTrace(TraceLevel.Information, "Combine Runtime Dependency: " + dependencyName);
						var dependencyTable = runtimeDependenciesTable[dependencyName].AsTable();

						if (dependencyTable.TryGetValue("Build", out var buildValue))
						{
							var dependencyBuildTable = buildValue.AsTable();

							if (dependencyBuildTable.TryGetValue("ModuleDependencies", out var moduleDependenciesValue))
							{
								var moduleDependencies = moduleDependenciesValue.AsList();
								buildTable.EnsureValueList("ModuleDependencies").Append(moduleDependencies);
							}

							if (dependencyBuildTable.TryGetValue("RuntimeDependencies", out var runtimeDependenciesValue))
							{
								var runtimeDependencies = runtimeDependenciesValue.AsList();
								buildTable.EnsureValueList("RuntimeDependencies").Append(runtimeDependencies);
							}

							if (dependencyBuildTable.TryGetValue("LinkDependencies", out var linkDependenciesValue))
							{
								var linkDependencies = linkDependenciesValue.AsList();
								buildTable.EnsureValueList("LinkDependencies").Append(linkDependencies);
							}
						}
					}
				}
			}
		}

		private static List<Path> CombineUnique(
			IEnumerable<Path> collection1,
			IEnumerable<Path> collection2)
		{
			var valueSet = new HashSet<string>();
			foreach (var value in collection1)
				valueSet.Add(value.ToString());
			foreach (var value in collection2)
				valueSet.Add(value.ToString());

			return valueSet.Select(value => new Path(value)).ToList();
		}

		private static List<Path> MakeUnique(IEnumerable<Path> collection)
		{
			var valueSet = new HashSet<string>();
			foreach (var value in collection)
				valueSet.Add(value.ToString());

			return valueSet.Select(value => new Path(value)).ToList();
		}
	}
}
