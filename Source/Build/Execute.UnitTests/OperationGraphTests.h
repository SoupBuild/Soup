// <copyright file="OperationGraphTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class OperationGraphTests
	{
	public:
		[[Fact]]
		void Initialize_Default()
		{
			auto uut = OperationGraph(1234);

			Assert::AreEqual(
				1234u,
				uut.GetStateId(),
				"Verify state id match expected.");
			Assert::AreEqual(
				std::unordered_map<CommandInfo, OperationInfo>(),
				uut.GetOperations(),
				"Verify operations match expected.");
		}

		[[Fact]]
		void Initialize_ListOperations_Single()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationInfo>({
					OperationInfo(
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({
							1,
						}),
						std::vector<FileId>({
							2,
						})),
				}));

			Assert::AreEqual(
				1234u,
				uut.GetStateId(),
				"Verify state id match expected.");
			Assert::AreEqual(
				std::unordered_map<CommandInfo, OperationInfo>({
					{
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						OperationInfo(
							CommandInfo(
								Path("C:/Root/"),
								Path("DoStuff.exe"),
								"arg1 arg2"),
							std::vector<FileId>({
								1,
							}),
							std::vector<FileId>({
								2,
							})),
					}
				}),
				uut.GetOperations(),
				"Verify operations match expected.");
		}

		[[Fact]]
		void TryFindOperationInfo_Missing()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationInfo>({}));

			const OperationInfo* operationInfo = nullptr;
			auto result = uut.TryFindOperationInfo(
				CommandInfo(
					Path("C:/Root/"),
					Path("DoStuff.exe"),
					"arg1 arg2"),
				operationInfo);

			Assert::IsFalse(result, "Verify result is false.");
			Assert::IsNull(operationInfo, "Verify operationInfo is null.");
		}

		[[Fact]]
		void TryFindOperationInfo_Found()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationInfo>({
					OperationInfo(
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({
							1,
						}),
						std::vector<FileId>({
							2,
						})),
				}));

			const OperationInfo* operationInfo;
			auto result = uut.TryFindOperationInfo(
				CommandInfo(
					Path("C:/Root/"),
					Path("DoStuff.exe"),
					"arg1 arg2"),
				operationInfo);

			Assert::IsTrue(result, "Verify result is true.");
			Assert::NotNull(operationInfo, "Verify operationInfo is not null.");
			Assert::AreEqual(
				OperationInfo(
					CommandInfo(
						Path("C:/Root/"),
						Path("DoStuff.exe"),
						"arg1 arg2"),
					std::vector<FileId>({
						1,
					}),
					std::vector<FileId>({
						2,
					})),
				*operationInfo,
				"Verify operationInfo is correct.");
		}

		[[Fact]]
		void AddOperationInfo()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationInfo>({}));

			uut.AddOperationInfo(
				OperationInfo(
					CommandInfo(
						Path("C:/Root/"),
						Path("DoStuff.exe"),
						"arg1 arg2"),
					std::vector<FileId>({
						1,
					}),
					std::vector<FileId>({
						2,
					})));

			Assert::AreEqual(
				std::unordered_map<CommandInfo, OperationInfo>({
					{
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						OperationInfo(
							CommandInfo(
								Path("C:/Root/"),
								Path("DoStuff.exe"),
								"arg1 arg2"),
							std::vector<FileId>({
								1,
							}),
							std::vector<FileId>({
								2,
							})),
					}
				}),
				uut.GetOperations(),
				"Verify operations match expected.");
		}
	};
}
