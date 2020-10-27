// <copyright file="SoupApiJsonModelsTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Api::UnitTests
{
	class SoupApiJsonModelsTests
	{
	public:
		// [[Fact]]
		void ParsePackageResult_GarbageThrows()
		{
			auto packageResult = std::string("garbage");
			Assert::ThrowsRuntimeError([&packageResult]() {
				auto actual = SoupApiJsonModels::ParsePackageResult(packageResult);
			});
		}

		// [[Fact]]
		void ParsePackageResult_MissingNameThrows()
		{
			auto packageResult = std::string(
				R"({
				})");

			Assert::ThrowsRuntimeError([&packageResult]() {
				auto actual = SoupApiJsonModels::ParsePackageResult(packageResult);
			});
		}

		// [[Fact]]
		void ParsePackageResult_Simple()
		{
			auto packageResult = std::string(
				R"({
					"name": "MyPackage"
				})");
			auto actual = SoupApiJsonModels::ParsePackageResult(packageResult);

			auto expected = PackageResultModel(
				"MyPackage");

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		// [[Fact]]
		void ParsePackageResult_AllProperties()
		{
			auto packageResult = std::string(
				R"({
					"name": "MyPackage",
					"latest": "1.2.3"
				})");
			auto actual = SoupApiJsonModels::ParsePackageResult(packageResult);

			auto expected = PackageResultModel(
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}
	};
}
