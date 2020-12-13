// <copyright file="RecipeBuildManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class RecipeBuildManagerTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto systemCompiler = "MockCompiler.System";
			auto runtimeCompiler = "MockCompiler.Runtime";
			auto arguments = RecipeBuildArguments();
			auto uut = RecipeBuildManager(systemCompiler, runtimeCompiler, arguments);
		}

		// TODO: Way more of this
	};
}
