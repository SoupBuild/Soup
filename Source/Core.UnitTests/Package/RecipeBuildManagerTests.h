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
			auto systemCompiler = std::make_shared<Compiler::Mock::Compiler>();
			auto runtimeCompiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = RecipeBuildManager(systemCompiler, runtimeCompiler);
		}

		[[Fact]]
		void Initialize_NullSystemCompilerThrows()
		{
			Assert::ThrowsRuntimeError([]() {
				auto systemCompiler = nullptr;
				auto runtimeCompiler = std::make_shared<Compiler::Mock::Compiler>();
				auto uut = RecipeBuildManager(systemCompiler, runtimeCompiler);
			});
		}

		[[Fact]]
		void Initialize_NullRuntimeCompilerThrows()
		{
			Assert::ThrowsRuntimeError([]() {
				auto systemCompiler = std::make_shared<Compiler::Mock::Compiler>();
				auto runtimeCompiler = nullptr;
				auto uut = RecipeBuildManager(systemCompiler, runtimeCompiler);
			});
		}

		// TODO: Way more of this
	};
}
