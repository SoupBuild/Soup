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
			auto packageGraphLookup = PackageGraphLookupMap();
			auto packageLookup = PackageLookupMap();
			auto uut = PackageProvider(1, packageGraphLookup, packageLookup);
		}
	};
}
