// <copyright file="BuildStateJsonTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class BuildStateJsonTests
	{
	public:
		[[Fact]]
		void Deserialize_GarbageThrows()
		{
			auto content = std::stringstream("garbage");
			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildStateJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_MissingKnownFiles()
		{
			auto content = std::stringstream(
				R"({
				})");

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildStateJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_MissingFileThrows()
		{
			auto content = std::stringstream(
				R"({
				   "knownFiles": [
					   {
						   "includes": []
					   }
				   ]
				})");

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildStateJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_MissingIncludesThrows()
		{
			auto content = std::stringstream(
				R"({
				   "knownFiles": [
					   {
						   "file": "File.h"
					   }
				   ]
				})");

			Assert::ThrowsRuntimeError([&content]() {
				auto actual = BuildStateJson::Deserialize(content);
			});
		}

		[[Fact]]
		void Deserialize_Simple()
		{
			auto content = std::stringstream(
				R"({
				   "knownFiles": [
					   {
						   "file": "File.h",
						   "includes": [ "Other.h" ]
					   }
				   ]
				})");
			auto actual = BuildStateJson::Deserialize(content);

			auto expected = BuildState(
				{
					FileInfo(Path("File.h"), { Path("Other.h") }),
				});

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Deserialize_Multiple()
		{
			auto content = std::stringstream(
				R"({
				   "knownFiles": [
					   {
						   "file": "File1.h",
						   "includes": [ "Other1.h" ]
					   },
					   {
						   "file": "File2.h",
						   "includes": [ "Other2.h" ]
					   }
				   ]
				})");
			auto actual = BuildStateJson::Deserialize(content);

			auto expected = BuildState(
				{
					FileInfo(Path("File1.h"), { Path("Other1.h") }),
					FileInfo(Path("File2.h"), { Path("Other2.h") }),
				});

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_Simple()
		{
			auto state = BuildState({
				FileInfo(Path("File.h"), { Path("Other.h") }),
			});

			std::stringstream actual;
			BuildStateJson::Serialize(state, actual);

			auto expected = 
				R"({
				   "knownFiles": [
					   {
						   "file": "File.h",
						   "includes": [ "Other.h" ]
					   }
				   ]
				})";

			VerifyJsonEquals(expected, actual.str(), "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_Multipl()
		{
			auto state = BuildState({
				FileInfo(Path("File1.h"), { Path("Other1.h") }),
				FileInfo(Path("File2.h"), { Path("Other2.h") }),
			});

			std::stringstream actual;
			BuildStateJson::Serialize(state, actual);

			auto expected = 
				R"({
				   "knownFiles": [
					   {
						   "file": "File1.h",
						   "includes": [ "Other1.h" ]
					   },
					   {
						   "file": "File2.h",
						   "includes": [ "Other2.h" ]
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
