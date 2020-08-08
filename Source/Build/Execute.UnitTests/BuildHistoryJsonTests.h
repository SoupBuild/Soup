// <copyright file="BuildHistoryJsonTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute::UnitTests
{
	class BuildHistoryJsonTests
	{
	public:
		[[Fact]]
		void Deserialize_GarbageThrows()
		{
			auto content = std::stringstream("garbage");
			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildHistoryJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_MissingOperationsThrows()
		{
			auto content = std::stringstream(
				R"({
				})");

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildHistoryJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_MissingCommandThrows()
		{
			auto content = std::stringstream(
				R"({
					"operations": [
						{
							"input": [
								"inputfile.txt"
							],
							"output": [
								"outputfile.txt"
							]
						}
					]
				})");

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildHistoryJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_MissingInputThrows()
		{
			auto content = std::stringstream(
				R"({
					"operations": [
						{
							"command": "./ : dostuff.exe arg1 arg2",
							"output": [
								"outputfile.txt"
							]
						}
					]
				})");

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildHistoryJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_MissingOutputThrows()
		{
			auto content = std::stringstream(
				R"({
					"operations": [
						{
							"command": "./ : dostuff.exe arg1 arg2",
							"input": [
								"inputfile.txt"
							],
						}
					]
				})");

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildHistoryJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_Simple()
		{
			auto content = std::stringstream(
				R"({
					"operations": [
						{
							"command": "./ : dostuff.exe arg1 arg2",
							"input": [
								"inputfile.txt"
							],
							"output": [
								"outputfile.txt"
							]
						}
					]
				})");
			auto actual = BuildHistoryJson::Deserialize(content);

			auto expected = BuildHistory(
				{
					OperationInfo(
						"./ : dostuff.exe arg1 arg2",
						{ Path("inputfile.txt") },
						{ Path("outputfile.txt") }),
				});

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Deserialize_Multiple()
		{
			auto content = std::stringstream(
				R"({
					"operations": [
						{
							"command": "./ : dostuff1.exe arg1 arg2",
							"input": [
								"inputfile1.txt"
							],
							"output": [
								"outputfile1.txt"
							]
						},
						{
							"command": "./ : dostuff2.exe arg1 arg2",
							"input": [
								"inputfile2.txt"
							],
							"output": [
								"outputfile2.txt"
							]
						}
					]
				})");
			auto actual = BuildHistoryJson::Deserialize(content);

			auto expected = BuildHistory(
				{
					OperationInfo(
						"./ : dostuff1.exe arg1 arg2",
						{ Path("inputfile1.txt") },
						{ Path("outputfile1.txt") }),
					OperationInfo(
						"./ : dostuff2.exe arg1 arg2",
						{ Path("inputfile2.txt") },
						{ Path("outputfile2.txt") }),
				});

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_Simple()
		{
			auto state = BuildHistory(
				{
					OperationInfo(
						"./ : dostuff.exe arg1 arg2",
						{ Path("inputfile.txt") },
						{ Path("outputfile.txt") }),
				});

			std::stringstream actual;
			BuildHistoryJson::Serialize(state, actual);

			auto expected = 
				R"({
					"operations": [
						{
							"command": "./ : dostuff.exe arg1 arg2",
							"input": [
								"inputfile.txt"
							],
							"output": [
								"outputfile.txt"
							]
						}
					]
				})";

			VerifyJsonEquals(expected, actual.str(), "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_Multipl()
		{
			auto state = BuildHistory(
				{
					OperationInfo(
						"./ : dostuff1.exe arg1 arg2",
						{ Path("inputfile1.txt") },
						{ Path("outputfile1.txt") }),
					OperationInfo(
						"./ : dostuff2.exe arg1 arg2",
						{ Path("inputfile2.txt") },
						{ Path("outputfile2.txt") }),
				});

			std::stringstream actual;
			BuildHistoryJson::Serialize(state, actual);

			auto expected = 
				R"({
					"operations": [
						{
							"command": "./ : dostuff1.exe arg1 arg2",
							"input": [
								"inputfile1.txt"
							],
							"output": [
								"outputfile1.txt"
							]
						},
						{
							"command": "./ : dostuff2.exe arg1 arg2",
							"input": [
								"inputfile2.txt"
							],
							"output": [
								"outputfile2.txt"
							]
						}
					]
				})";

			VerifyJsonEquals(expected, actual.str(), "Verify matches expected.");
		}

	private:
		static void VerifyJsonEquals(
			const std::string& expected,
			const std::string& actual,
			const std::string& message)
		{
			// Cleanup the expected json
			std::string error;
			auto jsonExpected = json11::Json::parse(expected, error);

			Assert::AreEqual(jsonExpected.dump(), actual, message);
		}
	};
}
