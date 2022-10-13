// <copyright file="RecipeBuildRunnerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "MockEvaluateEngine.h"

namespace Soup::Core::UnitTests
{
	class RecipeBuildRunnerTests
	{
	public:
		// [[Fact]]
		void Initialize_Success()
		{
			auto arguments = RecipeBuildArguments();
			auto sdkParameters = ValueList();
			auto sdkReadAccess = std::vector<Path>();
			auto hostBuildGlobalParameters = ValueTable();
			auto systemReadAccess = std::vector<Path>();
			auto recipeCache = RecipeCache();
			auto packageProvider = PackageProvider(std::map<int, PackageInfo>(), 1);
			auto evaluateEngine = MockEvaluateEngine();
			auto fileSystemState = FileSystemState();
			auto uut = RecipeBuildRunner(
				std::move(arguments),
				std::move(sdkParameters),
				std::move(sdkReadAccess),
				std::move(hostBuildGlobalParameters),
				std::move(systemReadAccess),
				recipeCache,
				packageProvider,
				evaluateEngine,
				fileSystemState);
		}
	};
}
