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
		void Initialize()
		{
			auto systemCompiler = "MockCompiler.System";
			auto runtimeCompiler = "MockCompiler.Runtime";
			auto uut = RecipeBuildManager(systemCompiler, runtimeCompiler);
		}

		// TODO: Way more of this
	};
}
