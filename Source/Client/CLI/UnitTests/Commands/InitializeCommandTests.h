// <copyright file="InitializeCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "InitializeCommand.h"

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Initialize Command Tests
	/// </summary>
	class InitializeCommandTests
	{
	public:
		// [[Fact]]
		void NameIsCorrect()
		{
			auto options = InitializeOptions();
			auto uut = InitializeCommand(options);
		}
	};
}
