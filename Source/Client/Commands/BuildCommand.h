// <copyright file="BuildCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "BuildOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Build Command
	/// </summary>
	class BuildCommand : public ICommand
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildCommand"/> class.
		/// </summary>
		BuildCommand(BuildOptions options) :
			_options(std::move(options))
		{
		}

		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() override final
		{
			Log::Diag("BuildCommand::Run");

			// Load the user config
			auto config =  LocalUserConfigExtensions::LoadFromFile();

			auto systemCompiler = std::make_shared<Compiler::Clang::Compiler>(
				Path(config.GetClangToolPath()));
			std::shared_ptr<ICompiler> runtimeCompiler = nullptr;
			if (config.GetRuntimeCompiler() == "clang")
			{
				runtimeCompiler = std::make_shared<Compiler::Clang::Compiler>(
					Path(config.GetClangToolPath()));
			}
			else if (config.GetRuntimeCompiler() == "msvc")
			{
				runtimeCompiler = std::make_shared<Compiler::MSVC::Compiler>(
					Path(config.GetMSVCRootPath()) + Path("bin/Hostx64/x64/"),
					Path("cl.exe"),
					Path("link.exe"),
					Path("lib.exe"));
			}
			else
			{
				throw std::runtime_error("Unknown compiler.");
			}

			auto workingDirectory = Path();
			if (_options.Path.empty())
			{
				// Buildin the current directory
				workingDirectory = Path::GetCurrentDirectory2();
			}
			else
			{
				workingDirectory = Path(_options.Path);

				// Check if this is relative to current directory
				if (!workingDirectory.HasRoot())
				{
					workingDirectory = Path::GetCurrentDirectory2() + workingDirectory;
				}
			}
			
			auto recipePath = 
				workingDirectory +
				Path(Constants::RecipeFileName);
			Recipe recipe = {};
			if (!RecipeExtensions::TryLoadFromFile(recipePath, recipe))
			{
				Log::Error("Could not load the recipe file.");
				return;
			}

			// Setup the build arguments
			auto arguments = RecipeBuildArguments();
			arguments.ForceRebuild = _options.Force;
			if (!_options.Configuration.empty())
				arguments.Configuration = _options.Configuration;
			else
				arguments.Configuration = "release";

			// TODO: Hard coded to windows MSVC runtime libraries
			// And we only trust the contig today
			arguments.PlatformIncludePaths = std::vector<Path>({});
			for (auto& path : config.GetWindowsSDKIncludePaths())
			{
				arguments.PlatformIncludePaths.push_back(Path(path));
			}

			arguments.PlatformLibraryPaths = std::vector<Path>({});
			for (auto& path : config.GetWindowsSDKLibraryPaths())
			{
				arguments.PlatformLibraryPaths.push_back(Path(path));
			}

			arguments.PlatformPreprocessorDefinitions = std::vector<std::string>({
				// "_DLL", // Link against the dynamic runtime dll
				// "_MT", // Use multithreaded runtime
			});

			// if (_options.Configuration == "debug")
			// {
			// 	// arguments.PlatformPreprocessorDefinitions.push_back("_DEBUG");
			// 	arguments.PlatformLibraries = std::vector<Path>({
			// 		Path("msvcprtd.lib"),
			// 		Path("msvcrtd.lib"),
			// 		Path("ucrtd.lib"),
			// 		Path("vcruntimed.lib"),
			// 	});
			// }
			// else
			// {
			// 	arguments.PlatformLibraries = std::vector<Path>({
			// 		Path("msvcprt.lib"),
			// 		Path("msvcrt.lib"),
			// 		Path("ucrt.lib"),
			// 		Path("vcruntime.lib"),
			// 	});
			// }

			// arguments.PlatformLibraries.push_back(Path("kernel32.lib"));
			// arguments.PlatformLibraries.push_back(Path("user32.lib"));
			// arguments.PlatformLibraries.push_back(Path("gdi32.lib"));
			// arguments.PlatformLibraries.push_back(Path("winspool.lib"));
			// arguments.PlatformLibraries.push_back(Path("comdlg32.lib"));
			// arguments.PlatformLibraries.push_back(Path("advapi32.lib"));
			// arguments.PlatformLibraries.push_back(Path("shell32.lib"));
			// arguments.PlatformLibraries.push_back(Path("ole32.lib"));
			// arguments.PlatformLibraries.push_back(Path("oleaut32.lib"));
			// arguments.PlatformLibraries.push_back(Path("uuid.lib"));
			// arguments.PlatformLibraries.push_back(Path("odbc32.lib"));
			// arguments.PlatformLibraries.push_back(Path("odbccp32.lib"));

			// Now build the current project
			Log::Info("Begin Build:");
			auto startTime = std::chrono::high_resolution_clock::now();

			auto buildManager = RecipeBuildManager(systemCompiler, runtimeCompiler);
			buildManager.Execute(workingDirectory, recipe, arguments);

			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime -startTime);

			Log::HighPriority(std::to_string(duration.count()) + " seconds.");
		}

	private:
		BuildOptions _options;
	};
}
