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
			auto systemReadAccess = std::vector<Path>();
			auto recipeCache = RecipeCache();
			auto packageProvider = PackageProvider(1, PackageGraphLookupMap(), PackageLookupMap());
			auto evaluateEngine = MockEvaluateEngine();
			auto fileSystemState = FileSystemState();
			auto uut = RecipeBuildRunner(
				std::move(arguments),
				std::move(sdkParameters),
				std::move(sdkReadAccess),
				std::move(systemReadAccess),
				recipeCache,
				packageProvider,
				evaluateEngine,
				fileSystemState);
		}
	};
}
