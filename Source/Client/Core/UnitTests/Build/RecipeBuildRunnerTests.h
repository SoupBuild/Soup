// <copyright file="RecipeBuildRunnerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

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
			auto packageProvider = PackageProvider();
			auto uut = RecipeBuildRunner(
				std::move(arguments),
				std::move(sdkParameters),
				std::move(sdkReadAccess),
				std::move(hostBuildGlobalParameters),
				std::move(systemReadAccess),
				std::move(packageProvider));
		}
	};
}
