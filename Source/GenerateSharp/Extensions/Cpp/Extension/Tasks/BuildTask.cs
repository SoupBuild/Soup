

using Soup.Build.Cpp.Compiler;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Cpp
{
	public class BuildTask : IBuildTask
	{
		private IBuildState _buildState;

		public BuildTask(IBuildState buildState)
		{
			_buildState = buildState;
		}

		public void Execute()
		{
			var activeState = _buildState.ActiveState;
			var sharedState = _buildState.SharedState;

			var buildTable = activeState["Build"].AsTable();
			var parametersTable = activeState["Parameters"].AsTable();

			var arguments = new BuildArguments();
			arguments.TargetArchitecture = parametersTable["Architecture"].AsString();
			arguments.TargetName = buildTable["TargetName"].AsString();
			arguments.TargetType = (BuildTargetType)
				buildTable["TargetType"].AsInteger();
			arguments.LanguageStandard = (LanguageStandard)
				buildTable["LanguageStandard"].AsInteger();
			arguments.WorkingDirectory = new Path(buildTable["WorkingDirectory"].AsString());
			arguments.ObjectDirectory = new Path(buildTable["ObjectDirectory"].AsString());
			arguments.BinaryDirectory = new Path(buildTable["BinaryDirectory"].AsString());

			if (buildTable.TryGetValue("ModuleInterfaceSourceFile", out var moduleInterfaceSourceFile))
			{
				arguments.ModuleInterfaceSourceFile = new Path(moduleInterfaceSourceFile.AsString());
			}

			if (buildTable.TryGetValue("Source", out var sourceValue))
			{
				arguments.SourceFiles = sourceValue.AsList().Select(value => new Path(value.AsString())).ToList();
			}

			if (buildTable.HasValue("IncludeDirectories"))
			{
				arguments.IncludeDirectories =
					buildTable.GetValue("IncludeDirectories").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("PlatformLibraries"))
			{
				arguments.PlatformLinkDependencies =
					buildTable.GetValue("PlatformLibraries").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("LinkLibraries"))
			{
				arguments.LinkDependencies =
					buildTable.GetValue("LinkLibraries").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("LibraryPaths"))
			{
				arguments.LibraryPaths =
					buildTable.GetValue("LibraryPaths").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("PreprocessorDefinitions"))
			{
				arguments.PreprocessorDefinitions =
					buildTable.GetValue("PreprocessorDefinitions").AsList().CopyAsStringVector();
			}

			if (buildTable.HasValue("OptimizationLevel"))
			{
				arguments.OptimizationLevel = static_cast<Soup::Cpp::Compiler::BuildOptimizationLevel>(
					buildTable.GetValue("OptimizationLevel").AsInteger().GetValue());
			}
			else
			{
				arguments.OptimizationLevel = Soup::Cpp::Compiler::BuildOptimizationLevel::None;
			}

			if (buildTable.HasValue("GenerateSourceDebugInfo"))
			{
				arguments.GenerateSourceDebugInfo =
					buildTable.GetValue("GenerateSourceDebugInfo").AsBoolean().GetValue();
			}
			else
			{
				arguments.GenerateSourceDebugInfo = false;
			}

			// Load the runtime dependencies
			if (buildTable.HasValue("RuntimeDependencies"))
			{
				arguments.RuntimeDependencies = MakeUnique(
					buildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector());
			}

			// Load the link dependencies
			if (buildTable.HasValue("LinkDependencies"))
			{
				arguments.LinkDependencies = MakeUnique(
					buildTable.GetValue("LinkDependencies").AsList().CopyAsPathVector());
			}

			// Load the module references
			if (buildTable.HasValue("ModuleDependencies"))
			{
				arguments.ModuleDependencies = MakeUnique(
					buildTable.GetValue("ModuleDependencies").AsList().CopyAsPathVector());
			}

			// Load the list of disabled warnings
			if (buildTable.HasValue("EnableWarningsAsErrors"))
			{
				arguments.EnableWarningsAsErrors =
					buildTable.GetValue("EnableWarningsAsErrors").AsBoolean().GetValue();
			}
			else
			{
				arguments.GenerateSourceDebugInfo = false;
			}

			// Load the list of disabled warnings
			if (buildTable.HasValue("DisabledWarnings"))
			{
				arguments.DisabledWarnings =
					buildTable.GetValue("DisabledWarnings").AsList().CopyAsStringVector();
			}

			// Check for any custom compiler flags
			if (buildTable.HasValue("CustomCompilerProperties"))
			{
				arguments.CustomProperties =
					buildTable.GetValue("CustomCompilerProperties").AsList().CopyAsStringVector();
			}

			// Initialize the compiler to use
			var compilerName = string(parametersTable.GetValue("Compiler").AsString().GetValue());
			var findCompilerFactory = _compilerFactory.find(compilerName);
			if (findCompilerFactory == _compilerFactory.end())
			{
				buildState.LogError("Unknown compiler: " + compilerName);
				return Soup::Build::ApiCallResult::Error;
			}

			var createCompiler = findCompilerFactory->second;
			var compiler = createCompiler(activeState);

			var buildEngine = Soup::Cpp::Compiler::BuildEngine(compiler);
			var buildResult = buildEngine.Execute(buildState, arguments);

			// Pass along internal state for other stages to gain access
			buildTable.EnsureValue("InternalLinkDependencies").EnsureList().SetAll(buildResult.InternalLinkDependencies);

			// Always pass along required input to shared build tasks
			var sharedBuildTable = sharedState.EnsureValue("Build").EnsureTable();
			sharedBuildTable.EnsureValue("ModuleDependencies").EnsureList().SetAll(buildResult.ModuleDependencies);
			sharedBuildTable.EnsureValue("RuntimeDependencies").EnsureList().SetAll(buildResult.RuntimeDependencies);
			sharedBuildTable.EnsureValue("LinkDependencies").EnsureList().SetAll(buildResult.LinkDependencies);

			if (!buildResult.TargetFile.IsEmpty())
			{
				sharedBuildTable.EnsureValue("TargetFile").SetValueString(buildResult.TargetFile.ToString());
			}

			// Register the build operations
			for (var & operation : buildResult.BuildOperations)
			{
				buildState.CreateOperation(operation);
			}

			buildState.LogInfo("Build Generate Done");
		}

		private static std::vector<Path> MakeUnique(const std::vector<Path>& collection)
		{
			std::unordered_set<string> valueSet;
			for (auto& value : collection)
				valueSet.insert(value.ToString());

			std::vector<Path> result;
			for (auto& value : valueSet)
				result.push_back(Path(value));

			return result;
		}
}
}
