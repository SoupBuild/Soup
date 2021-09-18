// <copyright file="BuildCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildCommand.h"

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Build Command Tests
	/// </summary>
	class BuildCommandTests
	{
	public:
		// [[Fact]]
		void NameIsCorrect()
		{
			auto options = BuildOptions();
			auto uut = BuildCommand(options);
		}
	};
}
