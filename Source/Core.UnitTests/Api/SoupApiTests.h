// <copyright file="SoupApiTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Api::UnitTests
{
	class SoupApiTests
	{
	public:
		[[Fact]]
		void Initialize_Success()
		{
			auto uut = SoupApi();
		}
	};
}
