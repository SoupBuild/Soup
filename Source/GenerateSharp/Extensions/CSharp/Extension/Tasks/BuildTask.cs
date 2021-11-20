

using Opal;
using Opal.System;
using Soup.Build.CSharp.Compiler;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.CSharp
{
	public class BuildTask : IBuildTask
	{
		private IBuildState buildState;
		private IValueFactory factory;
		private IDictionary<string, Func<IValueTable, ICompiler>> compilerFactory;

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
		};

		public BuildTask(IBuildState buildState, IValueFactory factory) :
			this(buildState, factory, new Dictionary<string, Func<IValueTable, ICompiler>>())
		{
			// Register default compilers
			// TODO: Fix up compiler names for different languages
			this.compilerFactory.Add("MSVC", (IValueTable activeState) =>
			{
				var clToolPath = new Path(activeState["Roslyn.CscToolPath"].AsString());
				return new Compiler.Roslyn.Compiler(clToolPath);
			});
		}

		public BuildTask(IBuildState buildState, IValueFactory factory, Dictionary<string, Func<IValueTable, ICompiler>> compilerFactory)
		{
			this.buildState = buildState;
			this.factory = factory;
			this.compilerFactory = compilerFactory;

			// TODO: Not ideal to be registering this on the fly...
			if (!LifetimeManager.Has<IProcessManager>())
			{
				LifetimeManager.RegisterSingleton<IProcessManager, RuntimeProcessManager>();
			}
		}

		public void Execute()
		{
			var activeState = this.buildState.ActiveState;
			var sharedState = this.buildState.SharedState;

			var buildTable = activeState["Build"].AsTable();
			var parametersTable = activeState["Parameters"].AsTable();

			var arguments = new BuildArguments();
			arguments.TargetArchitecture = parametersTable["Architecture"].AsString();
			arguments.TargetName = buildTable["TargetName"].AsString();
			arguments.TargetType = (BuildTargetType)buildTable["TargetType"].AsInteger();
			arguments.SourceRootDirectory = new Path(buildTable["SourceRootDirectory"].AsString());
			arguments.TargetRootDirectory = new Path(buildTable["TargetRootDirectory"].AsString());
			arguments.ObjectDirectory = new Path(buildTable["ObjectDirectory"].AsString());
			arguments.BinaryDirectory = new Path(buildTable["BinaryDirectory"].AsString());

			if (buildTable.TryGetValue("Source", out var sourceValue))
			{
				arguments.SourceFiles = sourceValue.AsList().Select(value => new Path(value.AsString())).ToList();
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

			if (buildTable.TryGetValue("NullableState", out var nullableValue))
			{
				arguments.NullableState = (BuildNullableState)nullableValue.AsInteger();
			}
			else
			{
				arguments.NullableState = BuildNullableState.Enabled;
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

			// Load the list of disabled warnings
			if (buildTable.TryGetValue("EnableWarningsAsErrors", out var enableWarningsAsErrorsValue))
			{
				arguments.EnableWarningsAsErrors = enableWarningsAsErrorsValue.AsBoolean();
			}
			else
			{
				arguments.GenerateSourceDebugInfo = false;
			}

			// Load the list of disabled warnings
			if (buildTable.TryGetValue("DisabledWarnings", out var disabledWarningsValue))
			{
				arguments.DisabledWarnings = disabledWarningsValue.AsList().Select(value => value.AsString()).ToList();
			}

			// Check for any custom compiler flags
			if (buildTable.TryGetValue("CustomCompilerProperties", out var customCompilerPropertiesValue))
			{
				arguments.CustomProperties = customCompilerPropertiesValue.AsList().Select(value => value.AsString()).ToList();
			}

			// Initialize the compiler to use
			var compilerName = parametersTable["Compiler"].AsString();
			if (!this.compilerFactory.TryGetValue(compilerName, out var compileFactory))
			{
				this.buildState.LogTrace(TraceLevel.Error, "Unknown compiler: " + compilerName);
				throw new InvalidOperationException();
			}

			var compiler = compileFactory(activeState);

			var buildEngine = new BuildEngine(compiler);
			var buildResult = buildEngine.Execute(this.buildState, arguments);

			// Pass along internal state for other stages to gain access
			buildTable.EnsureValueList(this.factory, "InternalLinkDependencies").SetAll(this.factory, buildResult.InternalLinkDependencies);

			// Always pass along required input to shared build tasks
			var sharedBuildTable = sharedState.EnsureValueTable(this.factory, "Build");
			sharedBuildTable.EnsureValueList(this.factory, "RuntimeDependencies").SetAll(this.factory, buildResult.RuntimeDependencies);
			sharedBuildTable.EnsureValueList(this.factory, "LinkDependencies").SetAll(this.factory, buildResult.LinkDependencies);

			if (!buildResult.TargetFile.IsEmpty)
			{
				sharedBuildTable["TargetFile"] = this.factory.Create(buildResult.TargetFile.ToString());
				sharedBuildTable["RunExecutable"] = this.factory.Create("C:/Program Files/dotnet/dotnet.exe");
				sharedBuildTable["RunArguments"] = this.factory.Create(buildResult.TargetFile.ToString());
			}

			// Register the build operations
			foreach (var operation in buildResult.BuildOperations)
			{
				this.buildState.CreateOperation(operation);
			}

			this.buildState.LogTrace(TraceLevel.Information, "Build Generate Done");
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
