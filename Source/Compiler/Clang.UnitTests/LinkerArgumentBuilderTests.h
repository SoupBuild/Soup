// <copyright file="LinkerArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::Clang::UnitTests
{
	class LinkerArgumentBuilderTests
	{
	public:
		[[Fact]]
		void ZeroObjectFiles()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.a");
			arguments.ObjectFiles = std::vector<Path>({});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"rc",
				"Library.mock.a",
			});
			auto expectedInput = std::vector<Path>({
			});
			auto expectedOutput = std::vector<Path>({
				Path("Library.mock.a"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void EmptyTargetFile_Throws()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			Assert::ThrowsRuntimeError([&arguments]() {
				auto actualInput = std::vector<Path>();
				auto actualOutput = std::vector<Path>();
				auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);
			});
		}

		[[Fact]]
		void StaticLibrary()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.a");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"rc",
				"Library.mock.a",
				"File.mock.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.mock.o"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("Library.mock.a"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void DynamicLibrary()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::DynamicLibrary;
			arguments.TargetFile = Path("Library.mock.so");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/subsystem:console",
				"/dll",
				"/implib:\"Library.mock.lib\"",
				"/machine:X64",
				"/out:\"Library.mock.so\"",
				"File.mock.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.mock.o"),
			});
			auto expectedOutput = std::vector<Path>({
				// TODO: Path("Library.mock.lib"),
				Path("Library.mock.so"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void Executable()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::Executable;
			arguments.TargetFile = Path("out/Something.exe");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.a"),
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/subsystem:console",
				"/machine:X64",
				"/out:\"out/Something.exe\"",
				"Library.mock.a",
				"File.mock.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("Library.mock.a"),
				Path("File.mock.o"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("out/Something.exe"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}
	};
}
