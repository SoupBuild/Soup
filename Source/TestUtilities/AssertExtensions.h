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
			Build::Utilities::BuildOperationWrapper& expected,
			Build::Utilities::BuildOperationWrapper& actual)
		{
			Assert::AreEqual(
				expected.GetTitle(),
				actual.GetTitle(),
				"Verify build operation title match expected.");
			Assert::AreEqual(
				expected.GetExecutable(),
				actual.GetExecutable(),
				"Verify build operation executable match expected.");
			Assert::AreEqual(
				expected.GetArguments(),
				actual.GetArguments(),
				"Verify build operation arguments match expected.");
			Assert::AreEqual(
				expected.GetWorkingDirectory(),
				actual.GetWorkingDirectory(),
				"Verify build operation working directory match expected.");
			Assert::AreEqual(
				expected.GetInputFileList().CopyAsStringVector(),
				actual.GetInputFileList().CopyAsStringVector(),
				"Verify build operation input files match expected.");
			Assert::AreEqual(
				expected.GetOutputFileList().CopyAsStringVector(),
				actual.GetOutputFileList().CopyAsStringVector(),
				"Verify build operation output files match expected.");
			
			AreEqual(expected.GetChildList(), actual.GetChildList());
		}

		static void AreEqual(
			Memory::Reference<Build::Utilities::BuildOperation>& expected,
			Memory::Reference<Build::Utilities::BuildOperation>& actual)
		{
			AreEqual(
				Build::Utilities::BuildOperationWrapper(expected), 
				Build::Utilities::BuildOperationWrapper(actual));
		}

		static void AreEqual(
			Memory::Reference<Build::Utilities::BuildOperation>& expected,
			Build::Utilities::BuildOperationWrapper& actual)
		{
			AreEqual(
				Build::Utilities::BuildOperationWrapper(expected), 
				actual);
		}

		static void AreEqual(
			Build::Utilities::BuildOperationListWrapper& expected,
			Build::Utilities::BuildOperationListWrapper& actual)
		{
			Assert::AreEqual(
				expected.GetSize(),
				actual.GetSize(),
				"Verify build operations size match expected.");

			for (size_t i = 0; i < expected.GetSize(); i++)
			{
				AreEqual(expected.GetValueAt(i), actual.GetValueAt(i));
			}
		}

		static void AreEqual(
			std::vector<Memory::Reference<Build::Utilities::BuildOperation>>& expected,
			std::vector<Build::Utilities::BuildOperationWrapper>& actual)
		{
			Assert::AreEqual(
				expected.size(),
				actual.size(),
				"Verify build operations size match expected.");

			for (size_t i = 0; i < expected.size(); i++)
			{
				AreEqual(expected.at(i), actual.at(i));
			}
		}
	};
}