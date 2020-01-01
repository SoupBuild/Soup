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
	export class BuildTask : public IBuildTask, public Memory::ReferenceCounted
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
		BuildSystemResult Execute(IBuildState& buildState) noexcept override final
		{
			try
			{
				auto state = BuildStateWrapper(buildState);

				auto arguments = BuildArguments();
				arguments.TargetName = state.GetPropertyStringValue("TargetName");
				arguments.TargetType = static_cast<BuildTargetType>(state.GetPropertyIntegerValue("TargetType"));
				arguments.LanguageStandard = static_cast<LanguageStandard>(state.GetPropertyIntegerValue("LanguageStandard"));
				arguments.WorkingDirectory = Path(state.GetPropertyStringValue("WorkingDirectory"));
				arguments.ObjectDirectory = Path(state.GetPropertyStringValue("ObjectDirectory"));
				arguments.BinaryDirectory = Path(state.GetPropertyStringValue("BinaryDirectory"));

				if (state.HasPropertyValue("ModuleInterfaceSourceFile"))
					arguments.ModuleInterfaceSourceFile = Path(state.GetPropertyStringValue("ModuleInterfaceSourceFile"));

				if (state.HasPropertyStringList("SourceFiles"))
					arguments.SourceFiles = state.CopyPropertyStringListAsPathVector("SourceFiles");

				if (state.HasPropertyStringList("IncludeDirectories"))
					arguments.IncludeDirectories = state.CopyPropertyStringListAsPathVector("IncludeDirectories");

				if (state.HasPropertyStringList("IncludeModules"))
					arguments.IncludeModules = state.CopyPropertyStringListAsPathVector("IncludeModules");

				if (state.HasPropertyStringList("LinkLibraries"))
					arguments.LinkLibraries = state.CopyPropertyStringListAsPathVector("LinkLibraries");

				if (state.HasPropertyStringList("ExternalLinkLibraries"))
					arguments.ExternalLinkLibraries = state.CopyPropertyStringListAsPathVector("ExternalLinkLibraries");

				if (state.HasPropertyStringList("LibraryPaths"))
					arguments.LibraryPaths = state.CopyPropertyStringListAsPathVector("LibraryPaths");

				if (state.HasPropertyStringList("PreprocessorDefinitions"))
					arguments.PreprocessorDefinitions = state.CopyPropertyStringListAsStringVector("PreprocessorDefinitions");

				if (state.HasPropertyValue("OptimizationLevel"))
					arguments.OptimizationLevel = static_cast<BuildOptimizationLevel>(state.GetPropertyIntegerValue("OptimizationLevel"));
				else
					arguments.OptimizationLevel = BuildOptimizationLevel::None;

				if (state.HasPropertyValue("GenerateSourceDebugInfo"))
					arguments.GenerateSourceDebugInfo = state.GetPropertyBooleanValue("GenerateSourceDebugInfo");
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
				Log::Diag("IncludeModules = " + ToString(arguments.IncludeModules));
				Log::Diag("PreprocessorDefinitions = " + ToString(arguments.PreprocessorDefinitions));

				// Copy previous runtime dependencies
				auto copyRuntimeDependencies = CopyRuntimeDependencies(state, arguments);
				for (auto& node : copyRuntimeDependencies)
				{
					state.AddBuildNode(node);
				}

				// Perform the core compilation of the source files
				auto compileNodes = CoreCompile(arguments);
				for (auto& node : compileNodes)
				{
					state.AddBuildNode(node);
				}

				// Link the final target after all of the compile graph is done
				auto linkNode = CoreLink(state, arguments);
				BuildGraphNode::AddLeafChild(compileNodes, linkNode);

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
			BuildStateWrapper& state,
			const BuildArguments& arguments)
		{
			auto copyNodes = std::vector<std::shared_ptr<BuildGraphNode>>();
			if (arguments.TargetType == BuildTargetType::Executable ||
				arguments.TargetType == BuildTargetType::DynamicLibrary)
			{
				if (state.HasPropertyStringList("RuntimeDependencies"))
				{
					auto runtimeDependencies = state.CopyPropertyStringListAsPathVector("RuntimeDependencies");
					for (auto source : runtimeDependencies)
					{
						auto target = arguments.WorkingDirectory + arguments.BinaryDirectory + Path(source.GetFileName());
						auto node = BuildUtilities::CreateCopyFileNode(source, target);
						copyNodes.push_back(node);
					}
				}
			}

			return copyNodes;
		}

		/// <summary>
		/// Compile the module and source files
		/// </summary>
		std::vector<std::shared_ptr<BuildGraphNode>> CoreCompile(const BuildArguments& arguments)
		{
			auto buildSetupNodes = std::vector<std::shared_ptr<BuildGraphNode>>();

			// Ensure the output directories exists
			auto objectDirectry = arguments.WorkingDirectory + arguments.ObjectDirectory;
			auto binaryDirectry = arguments.WorkingDirectory + arguments.BinaryDirectory;
			buildSetupNodes.push_back(BuildUtilities::CreateCreateDirectoryNode(objectDirectry));
			buildSetupNodes.push_back(BuildUtilities::CreateCreateDirectoryNode(binaryDirectry));

			// Compile the module interface unit if present
			std::shared_ptr<BuildGraphNode> moduleCompileNode = nullptr;
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty())
			{
				moduleCompileNode = CompileModuleInterfaceUnit(arguments);

				// Perform the compile after the setup
				BuildGraphNode::AddLeafChild(buildSetupNodes, moduleCompileNode);
			}

			if (!arguments.SourceFiles.empty())
			{
				auto sourceCompileNodes = CompileSourceFiles(arguments);
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
		std::shared_ptr<BuildGraphNode> CompileModuleInterfaceUnit(const BuildArguments& arguments)
		{
			Log::Info("CompileModuleInterfaceUnit");

			// Setup the shared properties
			auto compileArguments = CompileArguments();
			compileArguments.Standard = arguments.LanguageStandard;
			compileArguments.Optimize = Convert(arguments.OptimizationLevel);
			compileArguments.RootDirectory = arguments.WorkingDirectory;
			compileArguments.PreprocessorDefinitions = {};
			compileArguments.IncludeDirectories = arguments.IncludeDirectories;
			compileArguments.IncludeModules = arguments.IncludeModules;
			compileArguments.GenerateIncludeTree = true;
			compileArguments.ExportModule = true;
			compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;
			compileArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;

			// Compile the individual translation unit
			const auto& file = arguments.ModuleInterfaceSourceFile;
			Log::Info("Generate Compile Node: " + file.ToString());
			compileArguments.SourceFile = file;
			compileArguments.TargetFile = arguments.ObjectDirectory + Path(file.GetFileName());
			compileArguments.TargetFile.SetFileExtension(_compiler->GetObjectFileExtension());

			auto compileNode = _compiler->CreateCompileNode(compileArguments);

			// Copy the binary module interface to the binary directory after compiles
			auto objectOutputModuleInterfaceFile = arguments.ObjectDirectory + Path(file.GetFileName());
			objectOutputModuleInterfaceFile.SetFileExtension(_compiler->GetModuleFileExtension());
			auto binaryOutputModuleInterfaceFile = arguments.BinaryDirectory + Path(arguments.TargetName + "." + std::string(_compiler->GetModuleFileExtension()));
			auto copyInterfaceNode = BuildUtilities::CreateCopyFileNode(
				arguments.WorkingDirectory + objectOutputModuleInterfaceFile,
				arguments.WorkingDirectory + binaryOutputModuleInterfaceFile);
			BuildGraphNode::AddLeafChild(compileNode, copyInterfaceNode);

			return compileNode;
		}

		/// <summary>
		/// Compile the supporting source files
		/// </summary>
		std::vector<std::shared_ptr<BuildGraphNode>> CompileSourceFiles(const BuildArguments& arguments)
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
			compileArguments.IncludeModules = arguments.IncludeModules;
			compileArguments.GenerateIncludeTree = true;
			compileArguments.ExportModule = false;
			compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;
			compileArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;

			// Add the module binary interface if present
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty())
			{
				auto moduleInterfaceFile = arguments.ObjectDirectory + Path(arguments.ModuleInterfaceSourceFile.GetFileName());
				moduleInterfaceFile.SetFileExtension(_compiler->GetModuleFileExtension());
				compileArguments.IncludeModules.push_back(moduleInterfaceFile);
			}

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
			BuildStateWrapper& state,
			const BuildArguments& arguments)
		{
			Log::Info("CoreLink");

			Path targetFile;
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
			linkArguments.LibraryFiles = arguments.LinkLibraries;
			linkArguments.ExternalLibraryFiles = arguments.ExternalLinkLibraries;
			linkArguments.RootDirectory = arguments.WorkingDirectory;
			linkArguments.LibraryPaths = arguments.LibraryPaths;
			linkArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;

			// Translate the target type into the link target
			switch (arguments.TargetType)
			{
				case BuildTargetType::StaticLibrary:
				{
					linkArguments.TargetType = LinkTarget::StaticLibrary;
					break;
				}
				case BuildTargetType::DynamicLibrary:
				{
					linkArguments.TargetType = LinkTarget::DynamicLibrary;

					// Add the DLL as a runtime dependency
					auto runtimeDependencies = std::vector<Path>();
					if (state.HasPropertyStringList("RuntimeDependencies"))
						runtimeDependencies = state.CopyPropertyStringListAsPathVector("RuntimeDependencies");

					runtimeDependencies.push_back(linkArguments.RootDirectory + linkArguments.TargetFile);
					state.SetPropertyStringList("RuntimeDependencies", runtimeDependencies);
					break;
				}
				case BuildTargetType::Executable:
				{
					linkArguments.TargetType = LinkTarget::Executable;
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
