// <copyright file="LocalUserConfigTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class LocalUserConfigTests
	{
	public:
		// [[Fact]]
		void InitializerDefault()
		{
			auto uut = LocalUserConfig();

			Assert::IsFalse(uut.HasSDKs(), "Verify has no SDKs.");
		}

		// [[Fact]]
		void OperatorEqualDefault()
		{
			auto uut = LocalUserConfig();

			Assert::AreEqual(LocalUserConfig(), uut, "Verify equal.");
		}
	};
}
