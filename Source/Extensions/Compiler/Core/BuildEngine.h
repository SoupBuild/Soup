// <copyright file="BuildEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildArguments.h"
#include "BuildResult.h"
#include "BuildUtilities.h"
#include "ICompiler.h"

namespace Soup::Compiler
{
	/// <summary>
	/// The build engine
	/// </summary>
	export class BuildEngine
	{
	public:
		BuildEngine(std::shared_ptr<ICompiler> compiler) :
			_compiler(std::move(compiler))
		{
		}

		/// <summary>
		/// Generate the required build nodes for the requested build
		/// </summary>
		BuildResult Execute(
			Soup::Build::Extensions::BuildStateWrapper& buildState,
			const BuildArguments& arguments)
		{
			auto result = BuildResult();

			// There is a bug in MSVC that requires all input module interface files,
			// Add a copy back into the parent list for now...
			// TODO: MSVC requires the entire closure of interfaces
			if (_compiler->GetName() == "MSVC")
			{
				result.ModuleDependencies = arguments.ModuleDependencies;
			}

			// Ensure the output directories exists as the first step
			auto objectDirectry = arguments.WorkingDirectory + arguments.ObjectDirectory;
			auto binaryDirectry = arguments.WorkingDirectory + arguments.BinaryDirectory;
			result.BuildNodes.push_back(
				BuildUtilities::CreateCreateDirectoryNode(buildState, objectDirectry));
			result.BuildNodes.push_back(
				BuildUtilities::CreateCreateDirectoryNode(buildState, binaryDirectry));

			// Perform the core compilation of the source files
			CoreCompile(buildState, arguments, result);

			// Link the final target after all of the compile graph is done
			CoreLink(buildState, arguments, result);

			// Copy previous runtime dependencies after linking has completed
			CopyRuntimeDependencies(buildState, arguments, result);

			return result;
		}

	private:
		/// <summary>
		/// Compile the module and source files
		/// </summary>
		void CoreCompile(
			Soup::Build::Extensions::BuildStateWrapper& buildState,
			const BuildArguments& arguments,
			BuildResult& result)
		{
			auto rootCompileNodes = std::vector<Soup::Build::Extensions::GraphNodeWrapper>();

			// Compile the module interface unit if present
			auto moduleCompileNode = Soup::Build::Extensions::GraphNodeWrapper();
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty())
			{
				CompileModuleInterfaceUnit(
					buildState,
					arguments,
					result);

				// Copy the binary module interface to the binary directory after compiling
				auto objectModuleInterfaceFile = 
					arguments.WorkingDirectory +
					arguments.ObjectDirectory +
					Path(arguments.ModuleInterfaceSourceFile.GetFileName());
				objectModuleInterfaceFile.SetFileExtension(_compiler->GetModuleFileExtension());
				auto binaryOutputModuleInterfaceFile =
					arguments.WorkingDirectory +
					arguments.BinaryDirectory +
					Path(arguments.TargetName + "." + std::string(_compiler->GetModuleFileExtension()));
				auto copyInterfaceNode = BuildUtilities::CreateCopyFileNode(
					buildState,
					objectModuleInterfaceFile,
					binaryOutputModuleInterfaceFile);
				Soup::Build::Extensions::GraphNodeExtensions::AddLeafChild(result.BuildNodes, copyInterfaceNode);

				// Add output module interface to the parent set of modules
				// This will allow the module implementation units access as well as downstream
				// dependencies to the public interface.
				result.ModuleDependencies.push_back(binaryOutputModuleInterfaceFile);
			}

			if (!arguments.SourceFiles.empty())
			{
				CompileSourceFiles(
					buildState,
					arguments,
					result);
			}
		}

		/// <summary>
		/// Compile the single module interface unit
		/// Returns true if the file was compiled
		/// </summary>
		void CompileModuleInterfaceUnit(
			Soup::Build::Extensions::BuildStateWrapper& buildState,
			const BuildArguments& arguments,
			BuildResult& result)
		{
			buildState.LogInfo("CompileModuleInterfaceUnit");

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
			compileArguments.IncludeModules = arguments.ModuleDependencies;
			compileArguments.GenerateIncludeTree = true;
			compileArguments.ExportModule = true;
			compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;
			compileArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;
			compileArguments.TargetFile = targetFile;

			// Compile the individual translation unit
			const auto& file = arguments.ModuleInterfaceSourceFile;
			buildState.LogInfo("Generate Compile Node: " + file.ToString());
			compileArguments.SourceFile = file;

			auto compileNode = _compiler->CreateCompileNode(buildState, compileArguments);

			// Run after the module interface unit compile
			Soup::Build::Extensions::GraphNodeExtensions::AddLeafChild(result.BuildNodes, compileNode);
		}

		/// <summary>
		/// Compile the supporting source files
		/// </summary>
		void CompileSourceFiles(
			Soup::Build::Extensions::BuildStateWrapper& buildState,
			const BuildArguments& arguments,
			BuildResult& result)
		{
			// Check if we can skip the whole dang thing
			buildState.LogInfo("Compiling source files");

			// Setup the shared properties
			auto compileArguments = CompileArguments();
			compileArguments.Standard = arguments.LanguageStandard;
			compileArguments.Optimize = Convert(arguments.OptimizationLevel);
			compileArguments.RootDirectory = arguments.WorkingDirectory;
			compileArguments.PreprocessorDefinitions = {};
			compileArguments.IncludeDirectories = arguments.IncludeDirectories;
			compileArguments.IncludeModules = arguments.ModuleDependencies;
			compileArguments.GenerateIncludeTree = true;
			compileArguments.ExportModule = false;
			compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;
			compileArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;

			// Include our own interface module file
			std::copy(
				result.ModuleDependencies.begin(),
				result.ModuleDependencies.end(),
				std::back_inserter(compileArguments.IncludeModules)); 

			// Compile the individual translation units
			auto buildNodes = std::vector<Soup::Build::Extensions::GraphNodeWrapper>();
			for (auto& file : arguments.SourceFiles)
			{
				buildState.LogInfo("Generate Compile Node: " + file.ToString());
				compileArguments.SourceFile = file;
				compileArguments.TargetFile = arguments.ObjectDirectory + Path(file.GetFileName());
				compileArguments.TargetFile.SetFileExtension(_compiler->GetObjectFileExtension());

				// Compile the file
				auto node = _compiler->CreateCompileNode(buildState, compileArguments);
				buildNodes.push_back(std::move(node));
			}

			// Run the core compile next
			Soup::Build::Extensions::GraphNodeExtensions::AddLeafChildren(result.BuildNodes, buildNodes);
		}

		/// <summary>
		/// Link the library
		/// </summary>
		void CoreLink(
			Soup::Build::Extensions::BuildStateWrapper& buildState,
			const BuildArguments& arguments,
			BuildResult& result)
		{
			buildState.LogInfo("CoreLink");

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

			buildState.LogInfo("Linking target");

			auto linkArguments = LinkArguments();

			linkArguments.TargetFile = std::move(targetFile);
			linkArguments.ImplementationFile = std::move(implementationFile);
			linkArguments.RootDirectory = arguments.WorkingDirectory;
			linkArguments.LibraryPaths = arguments.LibraryPaths;
			linkArguments.GenerateSourceDebugInfo = arguments.GenerateSourceDebugInfo;

			// Only resolve link libraries if not a library ourself
			if (arguments.TargetType != BuildTargetType::StaticLibrary)
			{
				linkArguments.ExternalLibraryFiles = arguments.PlatformLinkDependencies;
				linkArguments.LibraryFiles = arguments.LinkDependencies;
			}

			// Translate the target type into the link target
			// and determine what dependencies to inject into downstream builds

			// Pass along all runtime dependencies
			result.RuntimeDependencies = arguments.RuntimeDependencies;

			switch (arguments.TargetType)
			{
				case BuildTargetType::StaticLibrary:
				{
					linkArguments.TargetType = LinkTarget::StaticLibrary;
					
					// Add the library as a link dependency and all transitive libraries
					result.LinkDependencies = arguments.LinkDependencies;
					result.LinkDependencies.push_back(linkArguments.RootDirectory + linkArguments.TargetFile);
					break;
				}
				case BuildTargetType::DynamicLibrary:
				{
					linkArguments.TargetType = LinkTarget::DynamicLibrary;

					// Add the DLL as a runtime dependency
					result.RuntimeDependencies.push_back(linkArguments.RootDirectory + linkArguments.TargetFile);
					
					// Clear out all previous link dependencies and replace with the 
					// single implementation library for the DLL
					result.LinkDependencies.push_back(linkArguments.RootDirectory + linkArguments.ImplementationFile);
					break;
				}
				case BuildTargetType::Executable:
				{
					linkArguments.TargetType = LinkTarget::Executable;

					// All link dependencies stop here.
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
			buildState.LogInfo("Generate Link Node: " + linkArguments.TargetFile.ToString());
			auto linkNode = _compiler->CreateLinkNode(buildState, linkArguments);

			// Run the link node
			Soup::Build::Extensions::GraphNodeExtensions::AddLeafChild(result.BuildNodes, linkNode);
		}

		/// <summary>
		/// Copy runtime dependencies
		/// </summary>
		void CopyRuntimeDependencies(
			Soup::Build::Extensions::BuildStateWrapper& buildState,
			const BuildArguments& arguments,
			BuildResult& result)
		{
			if (arguments.TargetType == BuildTargetType::Executable ||
				arguments.TargetType == BuildTargetType::DynamicLibrary)
			{
				auto copyNodes = std::vector<Soup::Build::Extensions::GraphNodeWrapper>();
				for (auto source : arguments.RuntimeDependencies)
				{
					auto target = arguments.WorkingDirectory + arguments.BinaryDirectory + Path(source.GetFileName());
					auto node = BuildUtilities::CreateCopyFileNode(buildState, source, target);
					copyNodes.push_back(node);
				}
				
				if (!copyNodes.empty())
				{
					Soup::Build::Extensions::GraphNodeExtensions::AddLeafChildren(result.BuildNodes, copyNodes);
				}
			}
		}

		Soup::Compiler::OptimizationLevel Convert(BuildOptimizationLevel value)
		{
			switch (value)
			{
				case BuildOptimizationLevel::None:
					return Soup::Compiler::OptimizationLevel::None;
				case BuildOptimizationLevel::Speed:
					return Soup::Compiler::OptimizationLevel::Speed;
				case BuildOptimizationLevel::Size:
					return Soup::Compiler::OptimizationLevel::Size;
				default:
					throw std::runtime_error("Unknown BuildOptimizationLevel.");
			}
		}

	private:
		std::shared_ptr<ICompiler> _compiler;
	};
}
