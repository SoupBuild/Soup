// <copyright file="BuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICompiler.h"
#include "Build\BuildHistoryChecker.h"
#include "Build\BuildHistoryManager.h"
#include "Build\BuildArguments.h"

namespace Soup
{
	/// <summary>
	/// The build task
	/// </summary>
	export class BuildTask : public BuildEx::IBuildTask
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildTask"/> class.
		/// </summary>
		BuildTask(std::shared_ptr<ICompiler> compiler) :
			_stateChecker(),
			_compiler(std::move(compiler))
		{
			if (_compiler == nullptr)
				throw std::runtime_error("Argument Null: compiler");
		}

		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() override final
		{
			return "Build";
		}

		/// <summary>
		/// The Core build task
		/// </summary>
		void Execute(BuildEx::IBuildState& state) override final
		{
			auto arguments = BuildArguments();
			arguments.TargetName = std::any_cast<std::string>(state.GetProperty("TargetName"));
			arguments.TargetType = std::any_cast<BuildTargetType>(state.GetProperty("TargetType"));
			arguments.LanguageStandard = std::any_cast<LanguageStandard>(state.GetProperty("LanguageStandard"));
			arguments.WorkingDirectory = std::any_cast<Path>(state.GetProperty("WorkingDirectory"));
			arguments.ObjectDirectory = std::any_cast<Path>(state.GetProperty("ObjectDirectory"));
			arguments.BinaryDirectory = std::any_cast<Path>(state.GetProperty("BinaryDirectory"));
			arguments.ModuleInterfaceSourceFile = std::any_cast<Path>(state.GetProperty("ModuleInterfaceSourceFile"));
			arguments.SourceFiles = std::any_cast<std::vector<Path>>(state.GetProperty("SourceFiles"));
			arguments.IncludeDirectories = std::any_cast<std::vector<Path>>(state.GetProperty("IncludeDirectories"));
			arguments.IncludeModules = std::any_cast<std::vector<Path>>(state.GetProperty("IncludeModules"));
			arguments.LinkLibraries = std::any_cast<std::vector<Path>>(state.GetProperty("LinkLibraries"));
			arguments.ExternalLinkLibraries = std::any_cast<std::vector<Path>>(state.GetProperty("ExternalLinkLibraries"));
			arguments.LibraryPaths = std::any_cast<std::vector<Path>>(state.GetProperty("LibraryPaths"));
			arguments.PreprocessorDefinitions = std::any_cast<std::vector<std::string>>(state.GetProperty("PreprocessorDefinitions"));
			arguments.IsIncremental = std::any_cast<bool>(state.GetProperty("IsIncremental"));
			arguments.OptimizationLevel = std::any_cast<BuildOptimizationLevel>(state.GetProperty("OptimizationLevel"));
			arguments.GenerateSourceDebugInfo = std::any_cast<bool>(state.GetProperty("GenerateSourceDebugInfo"));

			// Log the incoming request for verbose logs
			Log::Diag("TargetName = " + arguments.TargetName);
			Log::Diag("TargetType = " + ToString(arguments.TargetType));
			Log::Diag("LanguageStandard = " + ToString(arguments.LanguageStandard));
			Log::Diag("WorkingDirectory = " + arguments.WorkingDirectory.ToString());
			Log::Diag("ObjectDirectory = " + arguments.ObjectDirectory.ToString());
			Log::Diag("BinaryDirectory = " + arguments.BinaryDirectory.ToString());
			Log::Diag("ModuleInterfaceSourceFile = " + arguments.ModuleInterfaceSourceFile.ToString());
			Log::Diag("IsIncremental = " + ToString(arguments.IsIncremental));
			Log::Diag("OptimizationLevel = " + ToString(arguments.OptimizationLevel));
			Log::Diag("GenerateSourceDebugInfo = " + ToString(arguments.GenerateSourceDebugInfo));
			Log::Diag("IncludeDirectories = " + ToString(arguments.IncludeDirectories));
			Log::Diag("IncludeModules = " + ToString(arguments.IncludeModules));
			Log::Diag("PreprocessorDefinitions = " + ToString(arguments.PreprocessorDefinitions));

			// Perform the core compilation of the source files
			bool sourceCompiled = CoreCompile(arguments);

			// Link the final target
			bool targetLinked = CoreLink(arguments, sourceCompiled);

			if (targetLinked)
				Log::HighPriority("Done");
			else
				Log::HighPriority("Up to date");
		}

	private:
		/// <summary>
		/// Compile the module and source files
		/// Returns true if any files were compiled
		/// </summary>
		bool CoreCompile(const BuildArguments& arguments)
		{
			// Load the previous build state if performing an incremental build
			BuildHistory buildHistory = {};
			bool forceBuild = !arguments.IsIncremental;
			if (arguments.IsIncremental)
			{
				Log::Diag("Loading previous build state");
				if (!BuildHistoryManager::TryLoadState(arguments.WorkingDirectory, buildHistory))
				{
					Log::Info("No previous state found, full rebuild required");
					buildHistory = BuildHistory();
					forceBuild = true;
				}
			}

			// Ensure the output directories exists
			auto objectDirectry = arguments.WorkingDirectory + arguments.ObjectDirectory;
			if (!System::IFileSystem::Current().Exists(objectDirectry))
			{
				Log::Info("Create Directory: " + arguments.ObjectDirectory.ToString());
				System::IFileSystem::Current().CreateDirectory2(objectDirectry);
			}

			auto binaryDirectry = arguments.WorkingDirectory + arguments.BinaryDirectory;
			if (!System::IFileSystem::Current().Exists(binaryDirectry))
			{
				Log::Info("Create Directory: " + arguments.BinaryDirectory.ToString());
				System::IFileSystem::Current().CreateDirectory2(binaryDirectry);
			}

			// Compile the module interface unit if present
			bool codeCompiled = false;
			if (!arguments.ModuleInterfaceSourceFile.IsEmpty())
			{
				auto moduleCompiled = CompileModuleInterfaceUnit(
					arguments,
					buildHistory,
					forceBuild);

				// Force recompile the source files if the module was compiled
				forceBuild |= moduleCompiled;
				codeCompiled |= moduleCompiled;
			}

			if (!arguments.SourceFiles.empty())
			{
				auto sourceCompiled = CompileSourceFiles(
					arguments,
					buildHistory,
					forceBuild);

				// Force link if any of the source was compiled
				codeCompiled |= sourceCompiled;
			}

			if (codeCompiled)
			{
				Log::Info("Saving updated build state");
				BuildHistoryManager::SaveState(arguments.WorkingDirectory, buildHistory);
			}

			return codeCompiled || forceBuild;
		}

		/// <summary>
		/// Compile the single module interface unit
		/// Returns true if the file was compiled
		/// </summary>
		bool CompileModuleInterfaceUnit(
			const BuildArguments& arguments,
			BuildHistory& buildHistory,
			bool forceBuild)
		{
			Log::Info("Task: CompileModuleInterfaceUnit");

			bool buildRequired = forceBuild;
			if (!forceBuild)
			{
				// Check if each source file is out of date and requires a rebuild
				Log::Info("Check for updated source");
				
				// Try to build up the closure of include dependencies
				auto inputClosure = std::vector<Path>();
				const auto& sourceFile = arguments.ModuleInterfaceSourceFile;
				if (buildHistory.TryBuildIncludeClosure(sourceFile, inputClosure))
				{
					// Include the source file itself
					inputClosure.push_back(sourceFile);

					// All modules are input dependencies
					std::copy(
						arguments.IncludeModules.begin(),
						arguments.IncludeModules.end(),
						std::back_inserter(inputClosure));

					// Build the expected object file
					auto outputFile = arguments.ObjectDirectory + Path(sourceFile.GetFileName());
					outputFile.SetFileExtension(_compiler->GetObjectFileExtension());

					// TODO: Include the module binary interface as output too

					// Check if any of the input files have changed since lsat build
					if (_stateChecker.IsOutdated(
						outputFile,
						inputClosure,
						arguments.WorkingDirectory))
					{
						// The file or a dependecy has changed
						buildRequired = true;
					}
					else
					{
						Log::Info("File up to date: " + sourceFile.ToString());
					}
				}
				else
				{
					// Could not determine the set of input files, not enough info to perform incremental build
					buildRequired = true;
				}
			}

			// Check if we can skip the whole dang thing
			if (buildRequired)
			{
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
				Log::HighPriority(file.ToString());
				compileArguments.SourceFile = file;
				compileArguments.TargetFile = arguments.ObjectDirectory + Path(file.GetFileName());
				compileArguments.TargetFile.SetFileExtension(_compiler->GetObjectFileExtension());

				auto result = _compiler->Compile(compileArguments);

				// Copy the binary module interface to the binary directory
				auto objectOutputModuleInterfaceFile = arguments.ObjectDirectory + Path(file.GetFileName());
				objectOutputModuleInterfaceFile.SetFileExtension(_compiler->GetModuleFileExtension());
				auto binaryOutputModuleInterfaceFile = arguments.BinaryDirectory + Path(arguments.TargetName + "." + std::string(_compiler->GetModuleFileExtension()));
				Log::Info("Copy: [" + objectOutputModuleInterfaceFile.ToString() + "] -> [" + binaryOutputModuleInterfaceFile.ToString() + "]");
				System::IFileSystem::Current().CopyFile2(
					arguments.WorkingDirectory + objectOutputModuleInterfaceFile,
					arguments.WorkingDirectory + binaryOutputModuleInterfaceFile);

				// Save the build state for the module file
				buildHistory.UpdateIncludeTree(result.HeaderIncludeFiles);

				return true;
			}
			else
			{
				Log::Info("Module up to date");
				return false;
			}
		}

		/// <summary>
		/// Compile the supporting source files
		/// Returns true if any files were compiled
		/// </summary>
		bool CompileSourceFiles(
			const BuildArguments& arguments,
			BuildHistory& buildHistory,
			bool force)
		{
			Log::Info("Task: CompileSourceFiles");

			auto source = std::vector<Path>();
			if (!force)
			{
				// Check if each source file is out of date and requires a rebuild
				Log::Info("Check for updated source");
				for (auto& sourceFile : arguments.SourceFiles)
				{
					// Try to build up the closure of include dependencies
					auto inputClosure = std::vector<Path>();
					if (buildHistory.TryBuildIncludeClosure(sourceFile, inputClosure))
					{
						// Include the source file itself
						inputClosure.push_back(sourceFile);

						// All modules are input dependencies
						std::copy(
							arguments.IncludeModules.begin(),
							arguments.IncludeModules.end(),
							std::back_inserter(inputClosure));

						// Build the expected object file
						auto outputFile = arguments.ObjectDirectory + Path(sourceFile.GetFileName());
						outputFile.SetFileExtension(_compiler->GetObjectFileExtension());

						// Check if any of the input files have changed since lsat build
						if (_stateChecker.IsOutdated(
							outputFile,
							inputClosure,
							arguments.WorkingDirectory))
						{
							// The file or a dependecy has changed
							source.push_back(sourceFile);
						}
						else
						{
							Log::Info("File up to date: " + sourceFile.ToString());
						}
					}
					else
					{
						// Could not determine the set of input files, not enough info to perform incremental build
						source.push_back(sourceFile);
					}
				}
			}
			else
			{
				// The build is forced, rebuild all input
				source = arguments.SourceFiles;
			}

			// Check if we can skip the whole dang thing
			if (!source.empty())
			{
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
				for (auto& file : source)
				{
					Log::HighPriority(file.ToString());
					compileArguments.SourceFile = file;
					compileArguments.TargetFile = arguments.ObjectDirectory + Path(file.GetFileName());
					compileArguments.TargetFile.SetFileExtension(_compiler->GetObjectFileExtension());

					// Compile the file
					auto result = _compiler->Compile(compileArguments);

					// Save the build state for the compiled files
					buildHistory.UpdateIncludeTree(result.HeaderIncludeFiles);
				}

				return true;
			}
			else
			{
				Log::Info("Objects up to date");
				return false;
			}
		}

		/// <summary>
		/// Link the library
		/// Returns true if the target was linked, otherwise returns false
		/// </summary>
		bool CoreLink(const BuildArguments& arguments, bool force)
		{
			Log::Info("Task: CoreLink");

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

			// Check if the output target is missing
			bool linkRequired = force;
			if (!force)
			{
				if (!System::IFileSystem::Current().Exists(arguments.WorkingDirectory + targetFile))
				{
					Log::Info("Link target does not exist: " + targetFile.ToString());
					linkRequired = true;
				}
			}

			if (linkRequired)
			{
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
						linkArguments.TargetType = LinkTarget::StaticLibrary;
						break;
					case BuildTargetType::DynamicLibrary:
						linkArguments.TargetType = LinkTarget::DynamicLibrary;
						break;
					case BuildTargetType::Executable:
						linkArguments.TargetType = LinkTarget::Executable;
						break;
					default:
						throw std::runtime_error("Unknown build target type.");
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

				// Ensure the binary directory exists
				// var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
				// if (!Directory.Exists(objectDirectry))
				// {
				//	 Directory.CreateDirectory(objectDirectry);
				// }

				// Perform the link
				Log::Info(linkArguments.TargetFile.ToString());
				_compiler->Link(linkArguments);

				return true;
			}
			else
			{
				Log::Info("Final target up to date");
				return false;
			}
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
		BuildHistoryChecker _stateChecker;
		std::shared_ptr<ICompiler> _compiler;
	};
}
