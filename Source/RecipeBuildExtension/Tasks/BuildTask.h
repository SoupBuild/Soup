// <copyright file="BuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace RecipeBuild
{
	export using CreateCompiler = std::function<std::shared_ptr<Soup::ICompiler>(Soup::Build::ValueTableWrapper&)>;
	export using CompilerFactory = std::map<std::string, CreateCompiler>;

	/// <summary>
	/// The build task
	/// </summary>
	export class BuildTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		BuildTask(CompilerFactory compilerFactory) :
			_compilerFactory(std::move(compilerFactory))
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
		/// The Core build task
		/// </summary>
		Soup::Build::OperationResult Execute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::BuildStateWrapper(buildState);

			try
			{
				return Execute(buildStateWrapper);
			}
			catch (const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return -3;
			}
			catch(...)
			{
				// Unknown error
				return -1;
			}
		}

	private:
		/// <summary>
		/// The Core build task
		/// </summary>
		Soup::Build::OperationResult Execute(
			Soup::Build::BuildStateWrapper& buildState)
		{
			auto activeState = buildState.GetActiveState();
			auto parentState = buildState.GetParentState();

			auto buildTable = activeState.GetValue("Build").AsTable();

			auto arguments = Soup::Build::BuildArguments();
			arguments.TargetName = buildTable.GetValue("TargetName").AsString().GetValue();
			arguments.TargetType = static_cast<Soup::Build::BuildTargetType>(
				buildTable.GetValue("TargetType").AsInteger().GetValue());
			arguments.LanguageStandard = static_cast<Soup::LanguageStandard>(
				buildTable.GetValue("LanguageStandard").AsInteger().GetValue());
			arguments.WorkingDirectory = Path(buildTable.GetValue("WorkingDirectory").AsString().GetValue());
			arguments.ObjectDirectory = Path(buildTable.GetValue("ObjectDirectory").AsString().GetValue());
			arguments.BinaryDirectory = Path(buildTable.GetValue("BinaryDirectory").AsString().GetValue());

			if (buildTable.HasValue("ModuleInterfaceSourceFile"))
			{
				arguments.ModuleInterfaceSourceFile =
					Path(buildTable.GetValue("ModuleInterfaceSourceFile").AsString().GetValue());
			}

			if (buildTable.HasValue("Source"))
			{
				arguments.SourceFiles =
					buildTable.GetValue("Source").AsList().CopyAsPathVector();
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
				arguments.OptimizationLevel = static_cast<Soup::Build::BuildOptimizationLevel>(
					buildTable.GetValue("OptimizationLevel").AsInteger().GetValue());
			}
			else
			{
				arguments.OptimizationLevel = Soup::Build::BuildOptimizationLevel::None;
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
				arguments.RuntimeDependencies = buildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector();
			}

			// Load the link dependencies
			if (buildTable.HasValue("LinkDependencies"))
			{
				arguments.LinkDependencies = buildTable.GetValue("LinkDependencies").AsList().CopyAsPathVector();
			}

			// Load the module references
			if (buildTable.HasValue("ModuleDependencies"))
			{
				arguments.ModuleDependencies = buildTable.GetValue("ModuleDependencies").AsList().CopyAsPathVector();
			}

			// Initialize the compiler to use
			auto compilerName = std::string(activeState.GetValue("CompilerName").AsString().GetValue());
			auto findCompilerFactory = _compilerFactory.find(compilerName);
			if (findCompilerFactory == _compilerFactory.end())
			{
				buildState.LogError("Unknown compiler: " + compilerName);
				return -2;
			}

			auto createCompiler = findCompilerFactory->second;
			auto compiler = createCompiler(activeState);

			auto buildEngine = Soup::Build::BuildEngine(compiler);
			auto buildResult = buildEngine.Execute(buildState, arguments);

			// Always pass along required input to parent build tasks
			auto parentBuildTable = parentState.EnsureValue("Build").EnsureTable();
			parentBuildTable.EnsureValue("ModuleDependencies").EnsureList().SetAll(buildResult.ModuleDependencies);
			parentBuildTable.EnsureValue("RuntimeDependencies").EnsureList().SetAll(buildResult.RuntimeDependencies);
			parentBuildTable.EnsureValue("LinkDependencies").EnsureList().SetAll(buildResult.LinkDependencies);

			// Register the root build tasks
			for (auto& node : buildResult.BuildNodes)
			{
				buildState.RegisterRootNode(node);
			}

			buildState.LogInfo("Build Generate Done");
			return 0;
		}

	private:
		CompilerFactory _compilerFactory;
	};
}
