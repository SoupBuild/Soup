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
		}

		[[Fact]]
		void InitializerAll()
		{
			auto uut = LocalUserConfig(
				"clang",
				"../msvc/",
				"../clang/");

			Assert::AreEqual("clang", uut.GetRuntimeCompiler(), "Verify runtime compiler is correct.");
			Assert::IsTrue(uut.HasMSVCRootPath(), "Verify has msvc root path.");
			Assert::AreEqual("../msvc/", uut.GetMSVCRootPath(), "Verify msvc root path is correct.");
			Assert::IsTrue(uut.HasClangToolPath(), "Verify has clang tool path.");
			Assert::AreEqual("../clang/", uut.GetClangToolPath(), "Verify clang tool path is correct.");
		}
	};
}
