// <copyright file="PackageProvider.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageLock/PackageLockExtensions.h"
#include "RecipeBuildArguments.h"
#include "Recipe/RecipeCache.h"
#include "Utils/HandledException.h"
#include "BuildConstants.h"

namespace Soup::Core
{
	export class PackageInfo
	{
	public:
		PackageInfo(
			int projectId,
			Path packageRoot,
			const Recipe& recipe,
			std::map<std::string, std::vector<std::pair<PackageReference, int>>> dependencies) :
			ProjectId(projectId),
			PackageRoot(std::move(packageRoot)),
			Recipe(recipe),
			Dependencies(std::move(dependencies))
		{
		}

		int ProjectId;
		Path PackageRoot;
		const Recipe& Recipe;
		std::map<std::string, std::vector<std::pair<PackageReference, int>>> Dependencies;
	};

	/// <summary>
	/// The package provider that maintains the in memory representation of all build state
	/// </summary>
	export class PackageProvider
	{
	private:
		const Path _builtInExtensionPath = Path("Extensions/");
		const std::string _builtInCppLanguage = "C++";
		const std::string _builtInCppExtensionVersion = "0.4.0";
		const Path _builtInCppExtensionPath = Path("Soup.Cpp/");
		const Path _builtInCppExtensionFilename = Path("Soup.Cpp.dll");
		const std::string _builtInCSharpLanguage = "C#";
		const std::string _builtInCSharpExtensionVersion = "0.7.0";
		const Path _builtInCSharpExtensionPath = Path("Soup.CSharp/");
		const Path _builtInCSharpExtensionFilename = Path("Soup.CSharp.dll");

		std::map<int, PackageInfo> _packageLookup;
		int _rootPackageId;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="PackageProvider"/> class.
		/// </summary>
		PackageProvider(std::map<int, PackageInfo> packageLookup, int rootPackageId) :
			_packageLookup(std::move(packageLookup)),
			_rootPackageId(rootPackageId)
		{
		}

		const PackageInfo& GetRootPackageInfo()
		{
			return GetPackageInfo(_rootPackageId);
		}

		const PackageInfo& GetPackageInfo(int projectId)
		{
			// The PackageInfo must already be loaded
			auto findPackageInfo = _packageLookup.find(projectId);
			if (findPackageInfo != _packageLookup.end())
			{
				return findPackageInfo->second;
			}
			else
			{
				throw std::runtime_error("ProjectId [" + std::to_string(projectId) + "] not found in lookup");
			}
		}

		Path GetLanguageExtensionPath(const Recipe& recipe)
		{
			auto name = recipe.GetLanguage().GetName();
			if (name == _builtInCSharpLanguage)
			{
				auto processFilename = System::IProcessManager::Current().GetCurrentProcessFileName();
				auto processDirectory = processFilename.GetParent();
				return processDirectory +
					_builtInExtensionPath +
					_builtInCSharpExtensionPath +
					Path(_builtInCSharpExtensionVersion) +
					_builtInCSharpExtensionFilename;
			}
			else if (name == _builtInCppLanguage)
			{
				auto processFilename = System::IProcessManager::Current().GetCurrentProcessFileName();
				auto processDirectory = processFilename.GetParent();
				return processDirectory +
					_builtInExtensionPath +
					_builtInCppExtensionPath +
					Path(_builtInCppExtensionVersion) +
					_builtInCppExtensionFilename;
			}
			else
			{
				throw std::runtime_error("Unknown language extension path");
			}
		}
	};
}
