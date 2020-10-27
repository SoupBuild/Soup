// <copyright file="LocalUserConfigJsonTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class LocalUserConfigJsonTests
	{
	public:
		// [[Fact]]
		void Deserialize_GarbageThrows()
		{
			auto localUserConfig = std::stringstream("garbage");
			Assert::ThrowsRuntimeError([&localUserConfig]() {
				auto actual = LocalUserConfigJson::Deserialize(localUserConfig);
			});
		}

		// [[Fact]]
		void Deserialize_MissingRuntimeCompilerThrows()
		{
			auto localUserConfig = std::stringstream(
				R"({
				})");

			Assert::ThrowsRuntimeError([&localUserConfig]() {
				auto actual = LocalUserConfigJson::Deserialize(localUserConfig);
			});
		}

		// [[Fact]]
		void Deserialize_Simple()
		{
			auto localUserConfig = std::stringstream(
				R"({
					"runtimeCompiler": "clang"
				})");
			auto actual = LocalUserConfigJson::Deserialize(localUserConfig);

			auto expected = LocalUserConfig(
				"clang",
				std::nullopt,
				std::nullopt,
				std::nullopt,
				std::nullopt);

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		// [[Fact]]
		void Deserialize_AllProperties()
		{
			auto localUserConfig = std::stringstream(
				R"({
					"runtimeCompiler": "clang",
					"msvc": "../msvc/",
					"clang": "../clang/",
					"windowsSDKIncludes": [
						"windowsSDK/Include/"
					],
					"windowsSDKLibraries": [
						"windowsSDK/Library/"
					]
				})");
			auto actual = LocalUserConfigJson::Deserialize(localUserConfig);

			auto expected = LocalUserConfig(
				"clang",
				"../msvc/",
				"../clang/",
				std::vector<std::string>({
					"windowsSDK/Include/",
				}),
				std::vector<std::string>({
					"windowsSDK/Library/",
				}));

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}
	};
}
