// <copyright file="BuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp
{
	/// <summary>
	/// The build task
	/// </summary>
	export class BuildTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		BuildTask()
		{
		}

		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "Build";
		}

		/// <summary>
		/// Get the run before list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunBeforeList() const noexcept override final
		{
			return _runBeforeList;
		}

		/// <summary>
		/// Get the run after list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunAfterList() const noexcept override final
		{
			return _runAfterList;
		}

		/// <summary>
		/// The Core build task
		/// </summary>
		Soup::Build::ApiCallResult TryExecute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::Utilities::BuildStateWrapper(buildState);

			try
			{
				return Execute(buildStateWrapper);
			}
			catch (const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return Soup::Build::ApiCallResult::Error;
			}
			catch(...)
			{
				// Unknown error
				return Soup::Build::ApiCallResult::Error;
			}
		}

	private:
		/// <summary>
		/// The Core build task
		/// </summary>
		Soup::Build::ApiCallResult Execute(
			Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			auto activeState = buildState.GetActiveState();
			auto sharedState = buildState.GetSharedState();

			auto buildTable = activeState.GetValue("Build").AsTable();

			auto arguments = Soup::CSharp::Compiler::BuildArguments();
			arguments.TargetArchitecture = activeState.GetValue("BuildArchitecture").AsString().GetValue();
			arguments.TargetName = buildTable.GetValue("TargetName").AsString().GetValue();
			arguments.TargetType = static_cast<Soup::CSharp::Compiler::BuildTargetType>(
				buildTable.GetValue("TargetType").AsInteger().GetValue());
			arguments.WorkingDirectory = Path(buildTable.GetValue("WorkingDirectory").AsString().GetValue());
			arguments.ObjectDirectory = Path(buildTable.GetValue("ObjectDirectory").AsString().GetValue());
			arguments.BinaryDirectory = Path(buildTable.GetValue("BinaryDirectory").AsString().GetValue());

			if (buildTable.HasValue("Source"))
			{
				arguments.SourceFiles =
					buildTable.GetValue("Source").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("LibraryFiles"))
			{
				arguments.LibraryFiles =
					buildTable.GetValue("LibraryFiles").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("PreprocessorDefinitions"))
			{
				arguments.PreprocessorDefinitions =
					buildTable.GetValue("PreprocessorDefinitions").AsList().CopyAsStringVector();
			}

			if (buildTable.HasValue("EnableOptimization"))
			{
				arguments.EnableOptimization = 
					buildTable.GetValue("EnableOptimization").AsBoolean().GetValue();
			}
			else
			{
				arguments.EnableOptimization = false;
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

			// Initialize the compiler to use
			auto roslynToolsRoot = activeState.GetValue("Roslyn.BinaryRoot").AsString().GetValue();
			auto compiler = std::make_shared<Soup::CSharp::Compiler::Compiler>(
				Path(roslynToolsRoot),
				Path("csc.exe"));

			auto buildEngine = Soup::CSharp::Compiler::BuildEngine(compiler);
			auto buildResult = buildEngine.Execute(buildState, arguments);

			// Always pass along required input to shared build tasks
			auto sharedBuildTable = sharedState.EnsureValue("Build").EnsureTable();
			sharedBuildTable.EnsureValue("RuntimeDependencies").EnsureList().SetAll(buildResult.RuntimeDependencies);
			sharedBuildTable.EnsureValue("LinkDependencies").EnsureList().SetAll(buildResult.LinkDependencies);

			// Register the build operations
			for (auto& operation : buildResult.BuildOperations)
			{
				buildState.CreateOperation(operation);
			}

			buildState.LogInfo("Build Generate Done");
			return Soup::Build::ApiCallResult::Success;
		}

		static std::vector<Path> MakeUnique(const std::vector<Path>& collection)
		{
			std::unordered_set<std::string> valueSet;
			for (auto& value : collection)
				valueSet.insert(value.ToString());

			std::vector<Path> result;
			for (auto& value : valueSet)
				result.push_back(Path(value));

			return result;
		}

	private:
		Soup::Build::Utilities::ReadOnlyStringList _runBeforeList;
		Soup::Build::Utilities::ReadOnlyStringList _runAfterList;
	};
}
