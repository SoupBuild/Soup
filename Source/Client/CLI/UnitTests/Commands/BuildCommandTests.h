// <copyright file="BuildCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "InitializeCommand.h"

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
			auto options = InitializeOptions();
			auto uut = InitializeCommand(options);
		}
	};
}
