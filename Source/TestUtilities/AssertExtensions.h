// <copyright file="AssertExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
	export class AssertExtensions
	{
	public:
		static void AreEqual(
			const std::shared_ptr<Build::BuildGraphNode>& expected,
			const std::shared_ptr<Build::BuildGraphNode>& actual)
		{
			Assert::AreEqual(
				expected->GetTitle(),
				actual->GetTitle(),
				"Verify build node title match expected.");
			Assert::AreEqual(
				expected->GetProgram().ToString(),
				actual->GetProgram().ToString(),
				"Verify build node program match expected.");
			Assert::AreEqual(
				expected->GetArguments(),
				actual->GetArguments(),
				"Verify build node arguments match expected.");
			Assert::AreEqual(
				expected->GetWorkingDirectory().ToString(),
				actual->GetWorkingDirectory().ToString(),
				"Verify build node working directory match expected.");
			Assert::AreEqual(
				expected->GetInputFiles(),
				actual->GetInputFiles(),
				"Verify build node input files match expected.");
			Assert::AreEqual(
				expected->GetOutputFiles(),
				actual->GetOutputFiles(),
				"Verify build node output files match expected.");
			
			AreEqual(expected->GetChildren(), actual->GetChildren());
		}

		static void AreEqual(
			const std::vector<std::shared_ptr<Build::BuildGraphNode>>& expected,
			const std::vector<std::shared_ptr<Build::BuildGraphNode>>& actual)
		{
			Assert::AreEqual(
				expected.size(),
				actual.size(),
				"Verify build nodes size match expected.");

			for (size_t i = 0; i < expected.size(); i++)
			{
				AreEqual(expected.at(i), actual.at(i));
			}
		}
	};
}