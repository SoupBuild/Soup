// <copyright file="LocalUserConfigTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class LocalUserConfigTests
	{
	public:
		[[Fact]]
		void InitializerDefault()
		{
			auto uut = LocalUserConfig();

			Assert::AreEqual("", uut.GetRuntimeCompiler(), "Verify runtime compiler is correct.");
			Assert::IsFalse(uut.HasMSVCRootPath(), "Verify has no msvc root path.");
			Assert::IsFalse(uut.HasClangToolPath(), "Verify has no clang tool path.");
			Assert::IsFalse(uut.HasWindowsSDKIncludePaths(), "Verify has no windows sdk include paths.");
			Assert::IsFalse(uut.HasWindowsSDKLibraryPaths(), "Verify has no windows sdk library paths.");
		}

		[[Fact]]
		void InitializerAll()
		{
			auto uut = LocalUserConfig(
				"clang",
				"../msvc/",
				"../clang/",
				std::vector<std::string>({
					"windowsSDK/Include/",
				}),
				std::vector<std::string>({
					"windowsSDK/Library/",
				}));

			Assert::AreEqual("clang", uut.GetRuntimeCompiler(), "Verify runtime compiler is correct.");
			Assert::IsTrue(uut.HasMSVCRootPath(), "Verify has msvc root path.");
			Assert::AreEqual("../msvc/", uut.GetMSVCRootPath(), "Verify msvc root path is correct.");
			Assert::IsTrue(uut.HasClangToolPath(), "Verify has clang tool path.");
			Assert::AreEqual("../clang/", uut.GetClangToolPath(), "Verify clang tool path is correct.");
			Assert::IsTrue(uut.HasWindowsSDKIncludePaths(), "Verify has windows sdk include paths.");
			Assert::AreEqual(
				std::vector<std::string>({
					"windowsSDK/Include/",
				}),
				uut.GetWindowsSDKIncludePaths(),
				"Verify windows sdk include paths are correct.");
			Assert::IsTrue(uut.HasWindowsSDKLibraryPaths(), "Verify has windows sdk library paths.");
			Assert::AreEqual(
				std::vector<std::string>({
					"windowsSDK/Library/",
				}),
				uut.GetWindowsSDKLibraryPaths(),
				"Verify windows sdk library paths are correct.");
		}
	};
}
