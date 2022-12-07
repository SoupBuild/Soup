// <copyright file="OperationGraphTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class OperationGraphTests
	{
	public:
		// [[Fact]]
		void Initialize_Default()
		{
			auto uut = OperationGraph();

			Assert::AreEqual(
				std::vector<OperationId>(),
				uut.GetRootOperationIds(),
				"Verify root operation ids match expected.");
			Assert::AreEqual(
				std::unordered_map<OperationId, OperationInfo>(),
				uut.GetOperations(),
				"Verify operations match expected.");
		}

		// [[Fact]]
		void Initialize_ListOperations_Single()
		{
			auto uut = OperationGraph(
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
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						1),
				}));

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
							{ 1, },
							{ 2, },
							{ },
							{ },
							{ },
							1),
					},
				}),
				uut.GetOperations(),
				"Verify operations match expected.");
		}

		// [[Fact]]
		void SetRootOperationIds()
		{
			auto uut = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>({
					OperationInfo(
						1,
						"TestOperation",
						CommandInfo(
							Path("C:/Root/"),
							Path("DoStuff.exe"),
							"arg1 arg2"),
						{ 1, },
						{ 2 },
						{ },
						{ },
						{ },
						1),
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

		// [[Fact]]
		void GetOperationInfo_MissingThrows()
		{
			auto uut = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());

			auto exception = Assert::Throws<std::runtime_error>([&uut]() {
				auto operationInfo = uut.GetOperationInfo(1);
			});
			
			Assert::AreEqual("The provided operation id does not exist", exception.what(), "Verify Exception message");
		}

		// [[Fact]]
		void GetOperationInfo_Found()
		{
			auto uut = OperationGraph(
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
						{ 1, },
						{ 2, },
						{ },
						{ },
						{ },
						1),
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
					{ 1, },
					{ 2, },
					{ },
					{ },
					{ },
					1),
				operationInfo,
				"Verify operationInfo is correct.");
		}

		// [[Fact]]
		void AddOperation()
		{
			auto uut = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());

			uut.AddOperation(
				OperationInfo(
					1,
					"TestOperation",
					CommandInfo(
						Path("C:/Root/"),
						Path("DoStuff.exe"),
						"arg1 arg2"),
					{ 1, },
					{ 2, },
					{ },
					{ },
					{ },
					1));

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
							{ 1, },
							{ 2, },
							{ },
							{ },
							{ },
							1),
					}
				}),
				uut.GetOperations(),
				"Verify operations match expected.");
		}
	};
}
