// <copyright file="RecipeBuildManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class RecipeBuildManagerTests
	{
	public:
		[[Fact]]
		void Initialize_Success()
		{
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = RecipeBuildManager(compiler);
		}

		[[Fact]]
		void Initialize_NullCompilerThrows()
		{
			Assert::ThrowsRuntimeError([]() {
				auto uut = RecipeBuildManager(nullptr);
			});
		}

		// TODO: Way more of this
	};
}
