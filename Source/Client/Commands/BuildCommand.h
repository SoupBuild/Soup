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

			auto workingDirectory = Path();
			if (_options.Path.empty())
			{
				// Buildin the current directory
				workingDirectory = System::IFileSystem::Current().GetCurrentDirectory2();
			}
			else
			{
				workingDirectory = Path(_options.Path);

				// Check if this is relative to current directory
				if (!workingDirectory.HasRoot())
				{
					workingDirectory = System::IFileSystem::Current().GetCurrentDirectory2() + workingDirectory;
				}
			}

			auto recipePath = 
				workingDirectory +
				Path(Constants::RecipeFileName);
			Build::Recipe recipe = {};
			if (!Build::RecipeExtensions::TryLoadFromFile(recipePath, recipe))
			{
				Log::Error("Could not load the recipe file.");
				return;
			}

			// Setup the build arguments
			auto arguments = RecipeBuildArguments();
			arguments.ForceRebuild = _options.Force;
			if (!_options.Flavor.empty())
				arguments.Flavor = _options.Flavor;
			else
				arguments.Flavor = "release";

			if (!_options.Platform.empty())
				arguments.Platform = _options.Platform;
			else
				arguments.Platform = "Windows"; // TODO: Pull current platform

			// TODO: Hard coded to windows MSVC runtime libraries
			// And we only trust the config today
			arguments.PlatformIncludePaths = std::vector<std::string>({});
			for (auto& path : config.GetWindowsSDKIncludePaths())
			{
				arguments.PlatformIncludePaths.push_back(path);
			}

			arguments.PlatformLibraryPaths = std::vector<std::string>({});
			for (auto& path : config.GetWindowsSDKLibraryPaths())
			{
				arguments.PlatformLibraryPaths.push_back(path);
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

			std::string runtimeCompiler = config.GetRuntimeCompiler();
			std::string systemCompiler = runtimeCompiler;

			// Now build the current project
			Log::Info("Begin Build:");
			auto startTime = std::chrono::high_resolution_clock::now();

			auto buildManager = Build::RecipeBuildManager(systemCompiler, runtimeCompiler);
			buildManager.Execute(workingDirectory, recipe, arguments);

			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime -startTime);

			Log::HighPriority(std::to_string(duration.count()) + " seconds.");
		}

	private:
		BuildOptions _options;
	};
}
