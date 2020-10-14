// <copyright file="OperationGraphTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
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
				std::vector<OperationId>(),
				uut.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationInfo>(),
				uut.GetOperations(),
				"Verify operations match expected.");
		}

		[[Fact]]
		void Initialize_ListOperations_Single()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationId>({
					1,
				}),
				std::vector<OperationInfo>({
					OperationInfo(
						1,
						"TestOperation",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({ 1, }),
						std::vector<FileId>({ 2, }),
						std::vector<OperationId>({}),
						1,
						false,
						std::vector<FileId>({}),
						std::vector<FileId>({})),
				}));

			Assert::AreEqual(
				1234u,
				uut.GetStateId(),
				"Verify state id match expected.");
			Assert::AreEqual(
				std::vector<OperationId>({
					1,
				}),
				uut.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationInfo>({
					{
						1,
						OperationInfo(
							1,
							"TestOperation",
							CommandInfo(
								Path("C:/Root/"),
								Path("DoStuff.exe"),
								"arg1 arg2"),
							std::vector<FileId>({ 1, }),
							std::vector<FileId>({ 2, }),
							std::vector<OperationId>({}),
							1,
							false,
							std::vector<FileId>({}),
							std::vector<FileId>({})),
					},
				}),
				uut.GetOperations(),
				"Verify operations match expected.");
		}

		[[Fact]]
		void SetRootOperationIds()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationId>({}),
				std::vector<OperationInfo>({
					OperationInfo(
						1,
						"TestOperation",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({ 1, }),
						std::vector<FileId>({ 2 }),
						std::vector<OperationId>({}),
						1,
						false,
						std::vector<FileId>({}),
						std::vector<FileId>({})),
				}));

			Assert::AreEqual(
				std::vector<OperationId>(),
				uut.GetRootOperationIds(),
				"Verify root operation ids match expected.");

			uut.SetRootOperationIds(
				std::vector<OperationId>({
					1,
				}));

			Assert::AreEqual(
				std::vector<OperationId>({
					1
				}),
				uut.GetRootOperationIds(),
				"Verify root operation ids match expected.");
		}

		[[Fact]]
		void TryFindOperationInfo_Missing()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationId>({}),
				std::vector<OperationInfo>({}));

			OperationInfo* operationInfo = nullptr;
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
				std::vector<OperationId>({
					1
				}),
				std::vector<OperationInfo>({
					OperationInfo(
						1,
						"TestOperation",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({ 1, }),
						std::vector<FileId>({ 2, }),
						std::vector<OperationId>({}),
						1,
						false,
						std::vector<FileId>({}),
						std::vector<FileId>({})),
				}));

			OperationInfo* operationInfo;
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
					1,
					"TestOperation",
					CommandInfo(
						Path("C:/Root/"),
						Path("DoStuff.exe"),
						"arg1 arg2"),
					std::vector<FileId>({ 1, }),
					std::vector<FileId>({ 2, }),
					std::vector<OperationId>({}),
					1,
					false,
					std::vector<FileId>({}),
					std::vector<FileId>({})),
				*operationInfo,
				"Verify operationInfo is correct.");
		}

		[[Fact]]
		void GetOperationInfo_MissingThrows()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationId>({}),
				std::vector<OperationInfo>({}));

			Assert::ThrowsRuntimeError([&uut]() {
				auto operationInfo = uut.GetOperationInfo(1);
			},
			"The provided operation id does not exist");
		}

		[[Fact]]
		void GetOperationInfo_Found()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationId>({
					1
				}),
				std::vector<OperationInfo>({
					OperationInfo(
						1,
						"TestOperation",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						std::vector<FileId>({ 1, }),
						std::vector<FileId>({ 2, }),
						std::vector<OperationId>({}),
						1,
						false,
						std::vector<FileId>({}),
						std::vector<FileId>({})),
				}));

			auto operationInfo = uut.GetOperationInfo(1);

			Assert::AreEqual(
				OperationInfo(
					1,
					"TestOperation",
					CommandInfo(
						Path("C:/Root/"),
						Path("DoStuff.exe"),
						"arg1 arg2"),
					std::vector<FileId>({ 1, }),
					std::vector<FileId>({ 2, }),
					std::vector<OperationId>({}),
					1,
					false,
					std::vector<FileId>({}),
					std::vector<FileId>({})),
				operationInfo,
				"Verify operationInfo is correct.");
		}

		[[Fact]]
		void AddOperation()
		{
			auto uut = OperationGraph(
				1234,
				std::vector<OperationId>({}),
				std::vector<OperationInfo>({}));

			uut.AddOperation(
				OperationInfo(
					1,
					"TestOperation",
					CommandInfo(
						Path("C:/Root/"),
						Path("DoStuff.exe"),
						"arg1 arg2"),
					std::vector<FileId>({ 1, }),
					std::vector<FileId>({ 2, }),
					std::vector<OperationId>({}),
					1,
					false,
					std::vector<FileId>({}),
					std::vector<FileId>({})));

			Assert::AreEqual(
				std::unordered_map<OperationId, OperationInfo>({
					{
						1,
						OperationInfo(
							1,
							"TestOperation",
							CommandInfo(
								Path("C:/Root/"),
								Path("DoStuff.exe"),
								"arg1 arg2"),
							std::vector<FileId>({ 1, }),
							std::vector<FileId>({ 2, }),
							std::vector<OperationId>({}),
							1,
							false,
							std::vector<FileId>({}),
							std::vector<FileId>({})),
					}
				}),
				uut.GetOperations(),
				"Verify operations match expected.");
		}
	};
}
