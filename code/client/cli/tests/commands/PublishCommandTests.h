// <copyright file="PublishCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PublishCommand.h"

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Publish Command Tests
	/// </summary>
	class PublishCommandTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto options = PublishOptions();
			auto uut = PublishCommand(options);
		}
	};
}
