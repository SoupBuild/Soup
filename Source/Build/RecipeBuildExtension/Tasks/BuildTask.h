// <copyright file="BuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildArguments.h"

namespace RecipeBuild
{
	export using CreateCompiler = std::function<std::shared_ptr<Soup::ICompiler>(Soup::Build::PropertyBagWrapper&)>;
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
			try
			{
				return Execute(Soup::Build::BuildStateWrapper(buildState));
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

			auto arguments = BuildArguments();
			arguments.TargetName = activeState.GetPropertyStringValue("TargetName");
			arguments.TargetType = static_cast<BuildTargetType>(activeState.GetPropertyIntegerValue("TargetType"));
			arguments.LanguageStandard = static_cast<Soup::LanguageStandard>(activeState.GetPropertyIntegerValue("LanguageStandard"));
			arguments.WorkingDirectory = Path(activeState.GetPropertyStringValue("WorkingDirectory"));
			arguments.ObjectDirectory = Path(activeState.GetPropertyStringValue("ObjectDirectory"));
			arguments.BinaryDirectory = Path(activeState.GetPropertyStringValue("BinaryDirectory"));

			if (activeState.HasPropertyValue("ModuleInterfaceSourceFile"))
				arguments.ModuleInterfaceSourceFile = Path(activeState.GetPropertyStringValue("ModuleInterfaceSourceFile"));

			if (activeState.HasPropertyStringList("SourceFiles"))
				arguments.SourceFiles = activeState.CopyPropertyStringListAsPathVector("SourceFiles");

			if (activeState.HasPropertyStringList("IncludeDirectories"))
				arguments.IncludeDirectories = activeState.CopyPropertyStringListAsPathVector("IncludeDirectories");

			if (activeState.HasPropertyStringList("LinkLibraries"))
				arguments.LinkLibraries = activeState.CopyPropertyStringListAsPathVector("LinkLibraries");

			if (activeState.HasPropertyStringList("LibraryPaths"))
				arguments.LibraryPaths = activeState.CopyPropertyStringListAsPathVector("LibraryPaths");

			if (activeState.HasPropertyStringList("PreprocessorDefinitions"))
				arguments.PreprocessorDefinitions = activeState.CopyPropertyStringListAsStringVector("PreprocessorDefinitions");

			if (activeState.HasPropertyValue("OptimizationLevel"))
				arguments.OptimizationLevel = static_cast<BuildOptimizationLevel>(activeState.GetPropertyIntegerValue("OptimizationLevel"));
			else
				arguments.OptimizationLevel = BuildOptimizationLevel::None;

			if (activeState.HasPropertyValue("GenerateSourceDebugInfo"))
				arguments.GenerateSourceDebugInfo = activeState.GetPropertyBooleanValue("GenerateSourceDebugInfo");
			else
				arguments.GenerateSourceDebugInfo = false;

			// Log the incoming request for verbose logs
			Log::Diag("TargetName = " + arguments.TargetName);
			Log::Diag("TargetType = " + ToString(arguments.TargetType));
			Log::Diag("LanguageStandard = " + ToString(arguments.LanguageStandard));
			Log::Diag("WorkingDirectory = " + arguments.WorkingDirectory.ToString());
			Log::Diag("ObjectDirectory = " + arguments.ObjectDirectory.ToString());
			Log::Diag("BinaryDirectory = " + arguments.BinaryDirectory.ToString());
			Log::Diag("ModuleInterfaceSourceFile = " + arguments.ModuleInterfaceSourceFile.ToString());
			Log::Diag("OptimizationLevel = " + ToString(arguments.OptimizationLevel));
			Log::Diag("GenerateSourceDebugInfo = " + Soup::ToString(arguments.GenerateSourceDebugInfo));
			Log::Diag("IncludeDirectories = " + Soup::ToString(arguments.IncludeDirectories));
			Log::Diag("PreprocessorDefinitions = " + Soup::ToString(arguments.PreprocessorDefinitions));

			// Load the runtime dependencies
			auto runtimeDependencies = std::vector<Path>();
			if (activeState.HasPropertyStringList("RuntimeDependencies"))
			{
				runtimeDependencies = activeState.CopyPropertyStringListAsPathVector("RuntimeDependencies");
			}

			// Load the link dependencies
			auto linkDependencies = std::vector<Path>();
			if (activeState.HasPropertyStringList("LinkDependencies"))
			{
				linkDependencies = activeState.CopyPropertyStringListAsPathVector("LinkDependencies");
			}

			// Load the module references
			auto moduleDependencies = std::vector<Path>();
			if (activeState.HasPropertyStringList("ModuleDependencies"))
			{
				moduleDependencies = activeState.CopyPropertyStringListAsPathVector("ModuleDependencies");
			}

			// Initialize the compiler to use
			auto compilerName = std::string(activeState.GetPropertyStringValue("Compiler"));
			auto findCompilerFactory = _compilerFactory.find(compilerName);
			if (findCompilerFactory == _compilerFactory.end())
			{
				Log::Error("Unknown compiler: " + compilerName);
				return -2;
			}

			auto createCompiler = findCompilerFactory->second;
			auto compiler = createCompiler(activeState);

			// Copy previous runtime dependencies
			auto copyRuntimeDependencies = CopyRuntimeDependencies(
				buildState,
				runtimeDependencies,
				arguments);
			for (auto& node : copyRuntimeDependencies)
			{
				buildState.RegisterRootNode(node);
			}

			// Perform the core compilation of the source files
			auto compileNodes = CoreCompile(buildState, *compiler, moduleDependencies, arguments);
			for (auto& node : compileNodes)
			{
				buildState.RegisterRootNode(node);
			}

			// Link the final target after all of the compile graph is done
			auto linkNode = CoreLink(
				buildState,
				*compiler,
				runtimeDependencies,
				linkDependencies,
				arguments);
			Soup::Build::GraphNodeExtensions::AddLeafChild(compileNodes, linkNode);

			// Always pass along required input to parent build tasks
			parentState.SetPropertyStringList("ModuleDependencies", moduleDependencies);
			parentState.SetPropertyStringList("RuntimeDependencies", runtimeDependencies);
			parentState.SetPropertyStringList("LinkDependencies", linkDependencies);


			Log::Info("Build Generate Done");
			return 0;
		}

		/// <summary>
		/// Copy runtime dependencies
		/// </summary>
		std::vector<Soup::Build::GraphNodeWrapper> CopyRuntimeDependencies(
			Soup::Build::BuildStateWrapper& buildState,
			const std::vector<Path>& runtimeDependencies,
			const BuildArguments& arguments)
		{
			auto copyNodes = std::vector<Soup::Build::GraphNodeWrapper>();
			if (arguments.TargetType == BuildTargetType::Executable ||
				arguments.TargetType == BuildTargetType::DynamicLibrary)
			{
				for (auto source : runtimeDependencies)
				{
					auto target = arguments.WorkingDirectory + arguments.BinaryDirectory + Path(source.GetFileName());
					auto node = Soup::Build::BuildUtilities::CreateCopyFileNode(buildState, source, target);
					copyNodes.push_back(node);
				}
			}

			return copyNodes;
		}

		/// <summary>
		/// Compile the module and source files
		/// </summary>
		std::vector<Soup::Build::GraphNodeWrapper> CoreCompile(
			Soup::Build::BuildStateWrapper& buildState,
			const Soup::ICompiler& compiler,
			std::vector<Path>& moduleDependencies,
			const BuildArguments& arguments)
		{
			auto buildSetupNodes = std::vector<Soup::Build::GraphNodeWrapper>();

			// Move the active module dependencies into an internal copy for compilation
			// Note: The passed in module dependencies will now be used as an out parameter
			// to be passed to the parent
			auto activeModuleDependencies = std::move(moduleDependencies);

			// There is a bug in MSVC that requires all input module interface files,
			// Add a copy back into the parent list for now...
			// TODO: MSVC requires the entire closure of interfaces
			if (compiler.GetName() == "MSVC")
			{
				moduleDependencies = activeModuleDependencies;
			}

			// Ensure the output directories exists
			auto objectDirectry = arguments.WorkingDirectory + arguments.ObjectDirectory;
			auto binaryDirectry = arguments.WorkingDirectory + arguments.BinaryDirectory;
			buildSetupNodes.push_back(
				Soup::Build::BuildUtilities::CreateCreateDirectoryNode(buildState, objectDirectry));
			buildSetupNodes.push_back(
				Soup::Build::BuildUtilities::CreateCreateDirectoryNode(buildState, binaryDirectry));

			// Compile the module interface unit if present
			auto moduleCompileNode = Soup::Build::GraphNodeWrapper();
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty())
			{
				auto moduleInterfaceFile = Path();
				moduleCompileNode = CompileModuleInterfaceUnit(
					buildState,
					compiler,
					activeModuleDependencies,
					moduleInterfaceFile,
					arguments);

				// Add the module interface file to the active
				activeModuleDependencies.push_back(moduleInterfaceFile);

				// Copy the binary module interface to the binary directory after compiles
				auto binaryOutputModuleInterfaceFile =
					arguments.WorkingDirectory +
					arguments.BinaryDirectory +
					Path(arguments.TargetName + "." + std::string(compiler.GetModuleFileExtension()));
				auto copyInterfaceNode = Soup::Build::BuildUtilities::CreateCopyFileNode(
					buildState,
					arguments.WorkingDirectory + moduleInterfaceFile,
					binaryOutputModuleInterfaceFile);
				Soup::Build::GraphNodeExtensions::AddLeafChild(moduleCompileNode, copyInterfaceNode);

				// Perform the compile after the setup
				Soup::Build::GraphNodeExtensions::AddLeafChild(buildSetupNodes, moduleCompileNode);

				// Add output module interface to the parent set of modules
				// This will allow the module implmenentation units access as well as downstream
				// dependencies to the public interface.
				moduleDependencies.push_back(binaryOutputModuleInterfaceFile);
			}

			if (!arguments.SourceFiles.empty())
			{
				auto sourceCompileNodes = CompileSourceFiles(
					buildState,
					compiler,
					activeModuleDependencies,
					arguments);
				if (moduleCompileNode.IsValid())
				{
					// Run after the module interface unit compile
					Soup::Build::GraphNodeExtensions::AddLeafChildren(moduleCompileNode, sourceCompileNodes);
				}
				else
				{
					// Run directly after build setup
					Soup::Build::GraphNodeExtensions::AddLeafChildren(buildSetupNodes, sourceCompileNodes);
				}
			}

			return buildSetupNodes;
		}

		/// <summary>
		/// Compile the single module interface unit
		/// Returns true if the file was compiled
		/// </summary>
		Soup::Build::GraphNodeWrapper CompileModuleInterfaceUnit(
			Soup::Build::BuildStateWrapper& buildState,
			const Soup::ICompiler& compiler,
			const std::vector<Path>& moduleDependencies,
			Path& moduleInterfaceFile,
			const BuildArguments& arguments)
		{
			Log::Info("CompileModuleInterfaceUnit");

			// Build up the target object file name
			auto targetFile = arguments.ObjectDirectory + Path(arguments.ModuleInterfaceSourceFile.GetFileName());
			targetFile.SetFileExtension(compiler.GetObjectFileExtension());

			// Setup the shared properties
			auto compileArguments = Soup::CompileArguments();
			compileArguments.Standard = arguments.LanguageStandard;
			compileArguments.Optimize = Convert(arguments.OptimizationLevel);
			compileArguments.RootDirectory = arguments.WorkingDirectory;
			compileArguments.PreprocessorDefinitions = {};
			compileArguments.IncludeDirectories = arguments.IncludeDirectories;
			compileArguments.IncludeModules = moduleDependencies;
			compileArguments.GenerateIncludeTree = true;
			compileArguments.ExportModule = true;
			compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;
			compileArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;
			compileArguments.TargetFile = targetFile;

			// Compile the individual translation unit
			const auto& file = arguments.ModuleInterfaceSourceFile;
			Log::Info("Generate Compile Node: " + file.ToString());
			compileArguments.SourceFile = file;

			auto compileNode = compiler.CreateCompileNode(buildState, compileArguments);

			// Return the interface file location
			moduleInterfaceFile = arguments.ObjectDirectory + Path(file.GetFileName());
			moduleInterfaceFile.SetFileExtension(compiler.GetModuleFileExtension());

			return compileNode;
		}

		/// <summary>
		/// Compile the supporting source files
		/// </summary>
		std::vector<Soup::Build::GraphNodeWrapper> CompileSourceFiles(
			Soup::Build::BuildStateWrapper& buildState,
			const Soup::ICompiler& compiler,
			const std::vector<Path>& moduleDependencies,
			const BuildArguments& arguments)
		{
			// Check if we can skip the whole dang thing
			Log::Info("Compiling source files");

			// Setup the shared properties
			auto compileArguments = Soup::CompileArguments();
			compileArguments.Standard = arguments.LanguageStandard;
			compileArguments.Optimize = Convert(arguments.OptimizationLevel);
			compileArguments.RootDirectory = arguments.WorkingDirectory;
			compileArguments.PreprocessorDefinitions = {};
			compileArguments.IncludeDirectories = arguments.IncludeDirectories;
			compileArguments.IncludeModules = moduleDependencies;
			compileArguments.GenerateIncludeTree = true;
			compileArguments.ExportModule = false;
			compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;
			compileArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;

			// Compile the individual translation units
			auto buildNodes = std::vector<Soup::Build::GraphNodeWrapper>();
			for (auto& file : arguments.SourceFiles)
			{
				Log::Info("Generate Compile Node: " + file.ToString());
				compileArguments.SourceFile = file;
				compileArguments.TargetFile = arguments.ObjectDirectory + Path(file.GetFileName());
				compileArguments.TargetFile.SetFileExtension(compiler.GetObjectFileExtension());

				// Compile the file
				auto node = compiler.CreateCompileNode(buildState, compileArguments);
				buildNodes.push_back(std::move(node));
			}

			return buildNodes;
		}

		/// <summary>
		/// Link the library
		/// </summary>
		Soup::Build::GraphNodeWrapper CoreLink(
			Soup::Build::BuildStateWrapper& buildState,
			const Soup::ICompiler& compiler,
			std::vector<Path>& runtimeDependencies,
			std::vector<Path>& linkDependencies,
			const BuildArguments& arguments)
		{
			Log::Info("CoreLink");

			Path targetFile;
			Path implementationFile;
			switch (arguments.TargetType)
			{
				case BuildTargetType::StaticLibrary:
					targetFile = 
						arguments.BinaryDirectory + 
						Path(arguments.TargetName + "." + std::string(compiler.GetStaticLibraryFileExtension()));
					break;
				case BuildTargetType::DynamicLibrary:
					targetFile = 
						arguments.BinaryDirectory + 
						Path(arguments.TargetName + "." + std::string(compiler.GetDynamicLibraryFileExtension()));
					implementationFile = 
						arguments.BinaryDirectory + 
						Path(arguments.TargetName + "." + std::string(compiler.GetStaticLibraryFileExtension()));
					break;
				case BuildTargetType::Executable:
					targetFile = 
						arguments.BinaryDirectory + 
						Path(arguments.TargetName + ".exe");
					break;
				default:
					throw std::runtime_error("Unknown build target type.");
			}

			Log::Info("Linking target");

			auto linkArguments = Soup::LinkArguments();

			linkArguments.TargetFile = std::move(targetFile);
			linkArguments.ImplementationFile = std::move(implementationFile);
			linkArguments.LibraryFiles = arguments.LinkLibraries;
			linkArguments.RootDirectory = arguments.WorkingDirectory;
			linkArguments.LibraryPaths = arguments.LibraryPaths;
			linkArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;

			// Only resolve link libraries if not a library ourself
			if (arguments.TargetType != BuildTargetType::StaticLibrary)
			{
				linkArguments.ExternalLibraryFiles = linkDependencies;
			}

			// Translate the target type into the link target
			switch (arguments.TargetType)
			{
				case BuildTargetType::StaticLibrary:
				{
					linkArguments.TargetType = Soup::LinkTarget::StaticLibrary;
					
					// Add the library as a link dependency
					linkDependencies.push_back(linkArguments.RootDirectory + linkArguments.TargetFile);
					break;
				}
				case BuildTargetType::DynamicLibrary:
				{
					linkArguments.TargetType = Soup::LinkTarget::DynamicLibrary;

					// Add the DLL as a runtime dependency
					runtimeDependencies.push_back(linkArguments.RootDirectory + linkArguments.TargetFile);
					
					// Clear out all previous link dependencies and replace with the 
					// single implementation library for the DLL
					linkDependencies.clear();
					linkDependencies.push_back(linkArguments.RootDirectory + linkArguments.ImplementationFile);
					break;
				}
				case BuildTargetType::Executable:
				{
					linkArguments.TargetType = Soup::LinkTarget::Executable;

					// Clear out all previous link dependencies
					linkDependencies.clear();
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown build target type.");
				}
			}

			// Build up the set of object files
			std::vector<Path> objectFiles;
			for (auto& sourceFile : arguments.SourceFiles)
			{
				auto objectFile = arguments.ObjectDirectory + Path(sourceFile.GetFileName());
				objectFile.SetFileExtension(compiler.GetObjectFileExtension());
				objectFiles.push_back(objectFile);
			}

			// Add the module interface object file if present
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty())
			{
				auto objectFile = arguments.ObjectDirectory + Path(arguments.ModuleInterfaceSourceFile.GetFileName());
				objectFile.SetFileExtension(compiler.GetObjectFileExtension());
				objectFiles.push_back(objectFile);
			}

			linkArguments.ObjectFiles = std::move(objectFiles);

			// Perform the link
			Log::Info("Generate Link Node: " + linkArguments.TargetFile.ToString());
			auto linkNode = compiler.CreateLinkNode(buildState, linkArguments);

			return linkNode;
		}

		Soup::OptimizationLevel Convert(BuildOptimizationLevel value)
		{
			switch (value)
			{
				case BuildOptimizationLevel::None:
					return Soup::OptimizationLevel::None;
				case BuildOptimizationLevel::Speed:
					return Soup::OptimizationLevel::Speed;
				case BuildOptimizationLevel::Size:
					return Soup::OptimizationLevel::Size;
				default:
					throw std::runtime_error("Unknown BuildOptimizationLevel.");
			}
		}

	private:
		CompilerFactory _compilerFactory;
	};
}
