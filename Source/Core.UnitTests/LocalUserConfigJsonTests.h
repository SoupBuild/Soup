// <copyright file="LocalUserConfigJsonTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class LocalUserConfigJsonTests
	{
	public:
		[[Fact]]
		void Deserialize_GarbageThrows()
		{
			auto localUserConfig = std::stringstream("garbage");
			Assert::ThrowsRuntimeError([&localUserConfig]() {
				auto actual = LocalUserConfigJson::Deserialize(localUserConfig);
			});
		}

		[[Fact]]
		void Deserialize_MissingRuntimeCompilerThrows()
		{
			auto localUserConfig = std::stringstream(
				R"({
				})");

			Assert::ThrowsRuntimeError([&localUserConfig]() {
				auto actual = LocalUserConfigJson::Deserialize(localUserConfig);
			});
		}

		[[Fact]]
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
				std::nullopt);

			Assert::AreEqual(expected.GetRuntimeCompiler(), actual.GetRuntimeCompiler(), "Verify matches expected.");
			Assert::IsFalse(actual.HasMSVCToolPath(), "Verify matches expected.");
			Assert::IsFalse(actual.HasClangToolPath(), "Verify matches expected.");
		}

		[[Fact]]
		void Deserialize_AllProperties()
		{
			auto localUserConfig = std::stringstream(
				R"({
					"runtimeCompiler": "clang",
					"msvc": "../msvc/",
					"clang": "../clang/"
				})");
			auto actual = LocalUserConfigJson::Deserialize(localUserConfig);

			auto expected = LocalUserConfig(
				"clang",
				"../msvc/",
				"../clang/");

			Assert::AreEqual(expected.GetRuntimeCompiler(), actual.GetRuntimeCompiler(), "Verify matches expected.");
			Assert::AreEqual(expected.GetMSVCToolPath(), actual.GetMSVCToolPath(), "Verify matches expected.");
			Assert::AreEqual(expected.GetClangToolPath(), actual.GetClangToolPath(), "Verify matches expected.");
		}
	};
}
