// <copyright file="BuildEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildRunner.h"
#include "BuildEvaluateEngine.h"
#include "BuildLoadEngine.h"
#include "LocalUserConfig/LocalUserConfigExtensions.h"

namespace Soup::Core
{
	/// <summary>
	/// The core build system that brings together the individual build components to build a single root package
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class BuildEngine
	{
	public:
		static std::map<std::string, KnownLanguage> GetKnownLanguages()
		{
			auto result = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					KnownLanguage("Cpp", "Soup.Cpp")
				},
				{
					"C#",
					KnownLanguage("CSharp", "Soup.CSharp")
				},
				{
					"Wren",
					KnownLanguage("Wren", "Soup.Wren")
				},
			});

			return result;
		}

		static std::map<std::string, std::map<std::string, SemanticVersion>> GetBuiltInPackages()
		{
			auto result = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"C++",
					{
						{
							"copy",
							SemanticVersion(1, 0, 0)
						},
						{
							"mkdir",
							SemanticVersion(1, 0, 0)
						},
					}
				},
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(0, 8, 2)
						},
						{
							"Soup.CSharp",
							SemanticVersion(0, 9, 0)
						},
						{
							"Soup.Wren",
							SemanticVersion(0, 2, 0)
						},
					}
				},
			});

			return result;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		static void Execute(const RecipeBuildArguments& arguments)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			// Load the local user config and any sdk content
			auto sdkParameters = ValueList();
			auto sdkReadAccess = std::vector<Path>();
			LoadLocalUserConfig(sdkParameters, sdkReadAccess);

			auto endTime = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

			// std::cout << "LoadLocalUserConfig: " << std::to_string(duration.count()) << " seconds." << std::endl;
			
			startTime = std::chrono::high_resolution_clock::now();

			// Load the system specific state
			auto hostBuildGlobalParameters = ValueTable();
			auto systemReadAccess = std::vector<Path>();
			LoadHostSystemState(hostBuildGlobalParameters, systemReadAccess);

			endTime = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

			// std::cout << "LoadSystemState: " << std::to_string(duration.count()) << " seconds." << std::endl;

			startTime = std::chrono::high_resolution_clock::now();

			// Generate the package build graph
			auto knownLanguages = GetKnownLanguages();
			auto builtInPackages = GetBuiltInPackages();
			auto recipeCache = RecipeCache();
			auto loadEngine = BuildLoadEngine(
				knownLanguages,
				builtInPackages,
				arguments,
				hostBuildGlobalParameters,
				recipeCache);
			auto packageProvider = loadEngine.Load();

			endTime = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

			// std::cout << "BuildLoadEngine: " << std::to_string(duration.count()) << " seconds." << std::endl;

			startTime = std::chrono::high_resolution_clock::now();

			// Initialize a shared File System State to cache file system access
			auto fileSystemState = FileSystemState();

			// Initialize a shared Evaluate Engine
			auto evaluateEngine = BuildEvaluateEngine(
				arguments.ForceRebuild,
				fileSystemState);

			// Initialize shared location manager
			auto locationManager = RecipeBuildLocationManager(knownLanguages);

			// Initialize the build runner that will perform the generate and evaluate phase
			// for each individual package
			auto buildRunner = BuildRunner(
				arguments,
				sdkParameters,
				sdkReadAccess,
				systemReadAccess,
				recipeCache,
				packageProvider,
				evaluateEngine,
				fileSystemState,
				locationManager);
			buildRunner.Execute();

			endTime = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);

			// std::cout << "BuildRunner: " << std::to_string(duration.count()) << " seconds." << std::endl;
		}

	private:
		/// <summary>
		/// Load Local User Config and process any known state
		/// </summary>
		static void LoadLocalUserConfig(
			ValueList& sdkParameters,
			std::vector<Path>& sdkReadAccess)
		{
			// Load the local user config
			auto localUserConfigPath = System::IFileSystem::Current().GetUserProfileDirectory() +
				BuildConstants::SoupLocalStoreDirectory() +
				BuildConstants::LocalUserConfigFileName();
			LocalUserConfig localUserConfig = {};
			if (!LocalUserConfigExtensions::TryLoadLocalUserConfigFromFile(localUserConfigPath, localUserConfig))
			{
				Log::Warning("Local User Config invalid");
			}

			// Process the SDKs
			if (localUserConfig.HasSDKs())
			{
				Log::Info("Checking SDKs for read access");
				auto sdks = localUserConfig.GetSDKs();
				for (auto& sdk : sdks)
				{
					auto sdkName = sdk.GetName();
					Log::Info("Found SDK: " + sdkName);
					if (sdk.HasSourceDirectories())
					{
						for (auto& sourceDirectory : sdk.GetSourceDirectories())
						{
							Log::Info("  Read Access: " + sourceDirectory.ToString());
							sdkReadAccess.push_back(sourceDirectory);
						}
					}

					auto sdkParameter = ValueTable();
					sdkParameter.emplace("Name", Value(sdkName));
					if (sdk.HasProperties())
					{
						sdkParameter.emplace(
							"Properties",
							RecipeBuildStateConverter::ConvertToBuildState(sdk.GetProperties()));
					}

					sdkParameters.push_back(std::move(sdkParameter));
				}
			}
		}

		static void LoadHostSystemState(
			ValueTable& hostBuildGlobalParameters,
			std::vector<Path>& systemReadAccess)
		{
			auto system = std::string("Windows");
			hostBuildGlobalParameters.emplace("System", Value(std::string(system)));

			// Allow read access from system directories
			systemReadAccess.push_back(
				Path("C:/Windows/"));
		}
	};
}
