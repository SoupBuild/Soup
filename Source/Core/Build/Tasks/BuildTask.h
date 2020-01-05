// <copyright file="BuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICompiler.h"
#include "BuildArguments.h"
#include "Build/BuildUtilities.h"

namespace Soup::Build
{
	/// <summary>
	/// The build task
	/// </summary>
	export class BuildTask : public Memory::ReferenceCounted<IBuildTask>
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildTask"/> class.
		/// </summary>
		BuildTask(std::shared_ptr<ICompiler> compiler) :
			_compiler(std::move(compiler))
		{
			if (_compiler == nullptr)
				throw std::runtime_error("Argument Null: compiler");
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
		OperationResult Execute(IBuildState& buildState) noexcept override final
		{
			try
			{
				auto activeState = PropertyBagWrapper(buildState.GetActiveState());
				auto parentState = PropertyBagWrapper(buildState.GetParentState());

				auto arguments = BuildArguments();
				arguments.TargetName = activeState.GetPropertyStringValue("TargetName");
				arguments.TargetType = static_cast<BuildTargetType>(activeState.GetPropertyIntegerValue("TargetType"));
				arguments.LanguageStandard = static_cast<LanguageStandard>(activeState.GetPropertyIntegerValue("LanguageStandard"));
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
				Log::Diag("GenerateSourceDebugInfo = " + ToString(arguments.GenerateSourceDebugInfo));
				Log::Diag("IncludeDirectories = " + ToString(arguments.IncludeDirectories));
				Log::Diag("PreprocessorDefinitions = " + ToString(arguments.PreprocessorDefinitions));

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

				// Copy previous runtime dependencies
				auto copyRuntimeDependencies = CopyRuntimeDependencies(runtimeDependencies, arguments);
				for (auto& node : copyRuntimeDependencies)
				{
					buildState.AddBuildNode(node);
				}

				// Perform the core compilation of the source files
				auto compileNodes = CoreCompile(moduleDependencies, arguments);
				for (auto& node : compileNodes)
				{
					buildState.AddBuildNode(node);
				}

				// Link the final target after all of the compile graph is done
				auto linkNode = CoreLink(runtimeDependencies, linkDependencies, arguments);
				BuildGraphNode::AddLeafChild(compileNodes, linkNode);

				// Always pass along required input to parent build tasks
				parentState.SetPropertyStringList("ModuleDependencies", moduleDependencies);
				parentState.SetPropertyStringList("RuntimeDependencies", runtimeDependencies);
				parentState.SetPropertyStringList("LinkDependencies", linkDependencies);


				Log::Info("Build Generate Done");
				return 0;
			}
			catch(...)
			{
				// Unknown error
				return -1;
			}
		}

	private:
		/// <summary>
		/// Copy runtime dependencies
		/// </summary>
		std::vector<std::shared_ptr<BuildGraphNode>> CopyRuntimeDependencies(
			const std::vector<Path>& runtimeDependencies,
			const BuildArguments& arguments)
		{
			auto copyNodes = std::vector<std::shared_ptr<BuildGraphNode>>();
			if (arguments.TargetType == BuildTargetType::Executable ||
				arguments.TargetType == BuildTargetType::DynamicLibrary)
			{
				for (auto source : runtimeDependencies)
				{
					auto target = arguments.WorkingDirectory + arguments.BinaryDirectory + Path(source.GetFileName());
					auto node = BuildUtilities::CreateCopyFileNode(source, target);
					copyNodes.push_back(node);
				}
			}

			return copyNodes;
		}

		/// <summary>
		/// Compile the module and source files
		/// </summary>
		std::vector<std::shared_ptr<BuildGraphNode>> CoreCompile(
			std::vector<Path>& moduleDependencies,
			const BuildArguments& arguments)
		{
			auto buildSetupNodes = std::vector<std::shared_ptr<BuildGraphNode>>();

			// Move the active module dependencies into an internal copy for compilation
			// Note: The passed in module dependencies will now be used as an out parameter
			// to be passed to the parent
			auto activeModuleDependencies = std::move(moduleDependencies);

			// There is a bug in MSVC that requires all input module interface files,
			// Add a copy back into the parent list for now...
			// TODO: MSVC requires the entire closure of interfaces
			if (_compiler->GetName() == "MSVC")
			{
				moduleDependencies = activeModuleDependencies;
			}

			// Ensure the output directories exists
			auto objectDirectry = arguments.WorkingDirectory + arguments.ObjectDirectory;
			auto binaryDirectry = arguments.WorkingDirectory + arguments.BinaryDirectory;
			buildSetupNodes.push_back(BuildUtilities::CreateCreateDirectoryNode(objectDirectry));
			buildSetupNodes.push_back(BuildUtilities::CreateCreateDirectoryNode(binaryDirectry));

			// Compile the module interface unit if present
			std::shared_ptr<BuildGraphNode> moduleCompileNode = nullptr;
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty())
			{
				auto moduleInterfaceFile = Path();
				moduleCompileNode = CompileModuleInterfaceUnit(
					activeModuleDependencies,
					moduleInterfaceFile,
					arguments);

				// Add the module interface file to the active
				activeModuleDependencies.push_back(moduleInterfaceFile);

				// Copy the binary module interface to the binary directory after compiles
				auto binaryOutputModuleInterfaceFile =
					arguments.WorkingDirectory +
					arguments.BinaryDirectory +
					Path(arguments.TargetName + "." + std::string(_compiler->GetModuleFileExtension()));
				auto copyInterfaceNode = BuildUtilities::CreateCopyFileNode(
					arguments.WorkingDirectory + moduleInterfaceFile,
					binaryOutputModuleInterfaceFile);
				BuildGraphNode::AddLeafChild(moduleCompileNode, copyInterfaceNode);

				// Perform the compile after the setup
				BuildGraphNode::AddLeafChild(buildSetupNodes, moduleCompileNode);

				// Add output module interface to the parent set of modules
				// This will allow the module implmenentation units access as well as downstream
				// dependencies to the public interface.
				moduleDependencies.push_back(binaryOutputModuleInterfaceFile);
			}

			if (!arguments.SourceFiles.empty())
			{
				auto sourceCompileNodes = CompileSourceFiles(activeModuleDependencies, arguments);
				if (moduleCompileNode != nullptr)
				{
					// Run after the module interface unit compile
					BuildGraphNode::AddLeafChildren(moduleCompileNode, sourceCompileNodes);
				}
				else
				{
					// Run directly after build setup
					BuildGraphNode::AddLeafChildren(buildSetupNodes, sourceCompileNodes);
				}
			}

			return buildSetupNodes;
		}

		/// <summary>
		/// Compile the single module interface unit
		/// Returns true if the file was compiled
		/// </summary>
		std::shared_ptr<BuildGraphNode> CompileModuleInterfaceUnit(
			const std::vector<Path>& moduleDependencies,
			Path& moduleInterfaceFile,
			const BuildArguments& arguments)
		{
			Log::Info("CompileModuleInterfaceUnit");

			// Build up the target object file name
			auto targetFile = arguments.ObjectDirectory + Path(arguments.ModuleInterfaceSourceFile.GetFileName());
			targetFile.SetFileExtension(_compiler->GetObjectFileExtension());

			// Setup the shared properties
			auto compileArguments = CompileArguments();
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

			auto compileNode = _compiler->CreateCompileNode(compileArguments);

			// Return the interface file location
			moduleInterfaceFile = arguments.ObjectDirectory + Path(file.GetFileName());
			moduleInterfaceFile.SetFileExtension(_compiler->GetModuleFileExtension());

			return compileNode;
		}

		/// <summary>
		/// Compile the supporting source files
		/// </summary>
		std::vector<std::shared_ptr<BuildGraphNode>> CompileSourceFiles(
			const std::vector<Path>& moduleDependencies,
			const BuildArguments& arguments)
		{
			// Check if we can skip the whole dang thing
			Log::Info("Compiling source files");

			// Setup the shared properties
			auto compileArguments = CompileArguments();
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
			auto buildNodes = std::vector<std::shared_ptr<BuildGraphNode>>();
			for (auto& file : arguments.SourceFiles)
			{
				Log::Info("Generate Compile Node: " + file.ToString());
				compileArguments.SourceFile = file;
				compileArguments.TargetFile = arguments.ObjectDirectory + Path(file.GetFileName());
				compileArguments.TargetFile.SetFileExtension(_compiler->GetObjectFileExtension());

				// Compile the file
				auto node = _compiler->CreateCompileNode(compileArguments);
				buildNodes.push_back(std::move(node));
			}

			return buildNodes;
		}

		/// <summary>
		/// Link the library
		/// </summary>
		std::shared_ptr<BuildGraphNode> CoreLink(
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
						Path(arguments.TargetName + "." + std::string(_compiler->GetStaticLibraryFileExtension()));
					break;
				case BuildTargetType::DynamicLibrary:
					targetFile = 
						arguments.BinaryDirectory + 
						Path(arguments.TargetName + "." + std::string(_compiler->GetDynamicLibraryFileExtension()));
					implementationFile = 
						arguments.BinaryDirectory + 
						Path(arguments.TargetName + "." + std::string(_compiler->GetStaticLibraryFileExtension()));
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

			auto linkArguments = LinkArguments();

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
					linkArguments.TargetType = LinkTarget::StaticLibrary;
					
					// Add the library as a link dependency
					linkDependencies.push_back(linkArguments.RootDirectory + linkArguments.TargetFile);
					break;
				}
				case BuildTargetType::DynamicLibrary:
				{
					linkArguments.TargetType = LinkTarget::DynamicLibrary;

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
					linkArguments.TargetType = LinkTarget::Executable;

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
				objectFile.SetFileExtension(_compiler->GetObjectFileExtension());
				objectFiles.push_back(objectFile);
			}

			// Add the module interface object file if present
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty())
			{
				auto objectFile = arguments.ObjectDirectory + Path(arguments.ModuleInterfaceSourceFile.GetFileName());
				objectFile.SetFileExtension(_compiler->GetObjectFileExtension());
				objectFiles.push_back(objectFile);
			}

			linkArguments.ObjectFiles = std::move(objectFiles);

			// Perform the link
			Log::Info("Generate Link Node: " + linkArguments.TargetFile.ToString());
			auto linkNode = _compiler->CreateLinkNode(linkArguments);

			return linkNode;
		}

		OptimizationLevel Convert(BuildOptimizationLevel value)
		{
			switch (value)
			{
				case BuildOptimizationLevel::None:
					return OptimizationLevel::None;
				case BuildOptimizationLevel::Speed:
					return OptimizationLevel::Speed;
				case BuildOptimizationLevel::Size:
					return OptimizationLevel::Size;
				default:
					throw std::runtime_error("Unknown BuildOptimizationLevel.");
			}
		}

	private:
		std::shared_ptr<ICompiler> _compiler;
	};
}
