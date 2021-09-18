// <copyright file="InstallCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "InstallCommand.h"

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Install Command Tests
	/// </summary>
	class InstallCommandTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto options = InstallOptions();
			auto uut = InstallCommand(options);
		}
	};
}
