// <copyright file="RecipeBuildLocationManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class RecipeBuildLocationManagerTests
	{
	public:
		// [[Fact]]
		void GetOutputDirectory_Simple()
		{
			auto targetDirectory = Core::RecipeBuildLocationManager::GetOutputDirectory(
				workingDirectory,
				recipe,
				globalParameters,
				projectManager);
		}
	};
}
