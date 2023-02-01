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
			auto packageGraphLookup = PackageGraphLookupMap(
				{
					{ 1, PackageGraph(1, 1, ValueTable()) },
				});
			auto packageRecipe = Recipe();
			auto packageLookup = PackageLookupMap(
				{
					{ 1, PackageInfo(1, Path(), packageRecipe, std::vector<Path>(), Path(), PackageChildrenMap()) },
				});
			auto uut = PackageProvider(1, packageGraphLookup, packageLookup);

			Assert::AreEqual(
				1,
				uut.GetRootPackageGraph().Id,
				"Verify root package graph matches expected.");
		}
	};
}
