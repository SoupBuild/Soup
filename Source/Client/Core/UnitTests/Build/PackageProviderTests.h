// <copyright file="PackageProviderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class PackageProviderTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto packageLookup = std::map<int, PackageInfo>();
			auto uut = PackageProvider(packageLookup, 0);
		}
	};
}
