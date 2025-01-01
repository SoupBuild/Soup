// <copyright file="VersionCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "VersionCommand.h"

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Version Command Tests
	/// </summary>
	class VersionCommandTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto options = VersionOptions();
			auto uut = VersionCommand(options);
		}
	};
}
