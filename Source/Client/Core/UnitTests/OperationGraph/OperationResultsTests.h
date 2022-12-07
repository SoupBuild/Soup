// <copyright file="OperationResultsTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class OperationResultsTests
	{
	public:
		// [[Fact]]
		void Initialize_Default()
		{
			auto uut = OperationResults();

			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>(),
				uut.GetResults(),
				"Verify results match expected.");
		}

		// [[Fact]]
		void Initialize_ListOperations_Single()
		{
			auto uut = OperationResults({
				{
					1,
					OperationResult(
						false,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::time_point<std::chrono::system_clock>()),
						{ },
						{ })
				},
			});

			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>({
					{
						1,
						OperationResult(
							false,
							std::chrono::clock_cast<std::chrono::file_clock>(
								std::chrono::time_point<std::chrono::system_clock>()),
							{ },
							{ })
					},
				}),
				uut.GetResults(),
				"Verify results match expected.");
		}

		// [[Fact]]
		void TryFindResult_Missing()
		{
			auto uut = OperationResults();

			OperationResult* operationResult = nullptr;
			auto result = uut.TryFindResult(1, operationResult);

			Assert::IsFalse(result, "Verify result is false.");
			Assert::IsNull(operationResult, "Verify operationResult is null.");
		}

		// [[Fact]]
		void TryFindResult_Found()
		{
			auto uut = OperationResults({
				{
					1,
					OperationResult(
						false,
						std::chrono::clock_cast<std::chrono::file_clock>(
							std::chrono::time_point<std::chrono::system_clock>()),
						{ },
						{ })
				},
			});

			OperationResult* operationResult;
			auto result = uut.TryFindResult(1, operationResult);

			Assert::IsTrue(result, "Verify result is true.");
			Assert::NotNull(operationResult, "Verify operationResult is not null.");
			Assert::AreEqual(
				OperationResult(
					false,
					std::chrono::clock_cast<std::chrono::file_clock>(
						std::chrono::time_point<std::chrono::system_clock>()),
					{ },
					{ }),
				*operationResult,
				"Verify operationResult is correct.");
		}

		// [[Fact]]
		void AddOrUpdateOperationResult()
		{
			auto uut = OperationResults();

			uut.AddOrUpdateOperationResult(
				1,
				OperationResult(
					false,
					std::chrono::clock_cast<std::chrono::file_clock>(
						std::chrono::time_point<std::chrono::system_clock>()),
					{ },
					{ }));

			Assert::AreEqual(
				std::unordered_map<OperationId, OperationResult>({
					{
						1,
						OperationResult(
							false,
							std::chrono::clock_cast<std::chrono::file_clock>(
								std::chrono::time_point<std::chrono::system_clock>()),
							{ },
							{ }),
					},
				}),
				uut.GetResults(),
				"Verify results match expected.");
		}
	};
}
