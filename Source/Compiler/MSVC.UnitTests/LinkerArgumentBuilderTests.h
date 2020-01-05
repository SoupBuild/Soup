// <copyright file="LinkerArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::MSVC::UnitTests
{
	class LinkerArgumentBuilderTests
	{
	public:
		[[Fact]]
		void ZeroObjectFiles()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/machine:X64",
				"/out:\"Library.mock.lib\"",
			});
			auto expectedInput = std::vector<Path>({
			});
			auto expectedOutput = std::vector<Path>({
				Path("Library.mock.lib"),
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
				Path("File.mock.obj"),
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
			arguments.TargetFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/machine:X64",
				"/out:\"Library.mock.lib\"",
				"File.mock.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.mock.o"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("Library.mock.lib"),
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
			Assert::AreEqual(expectedInput, actualInput, "Verify generated input match expected.");
			Assert::AreEqual(expectedOutput, actualOutput, "Verify generated output match expected.");
		}

		[[Fact]]
		void StaticLibrary_LibraryPaths()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});
			arguments.LibraryPaths = std::vector<Path>({
				Path("../libraries/"),
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/machine:X64",
				"/libpath:\"../libraries/\"",
				"/out:\"Library.mock.lib\"",
				"File.mock.o",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.mock.o"),
			});
			auto expectedOutput = std::vector<Path>({
				Path("Library.mock.lib"),
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
			arguments.TargetFile = Path("Library.mock.dll");
			arguments.ImplementationFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.obj"),
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
				"/out:\"Library.mock.dll\"",
				"File.mock.obj",
			});
			auto expectedInput = std::vector<Path>({
				Path("File.mock.obj"),
			});
			auto expectedOutput = std::vector<Path>({
				// TODO: Path("Library.mock.lib"),
				Path("Library.mock.dll"),
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
				Path("File.mock.obj"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.lib"),
			});

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments, actualInput, actualOutput);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/subsystem:console",
				"/machine:X64",
				"/out:\"out/Something.exe\"",
				"Library.mock.lib",
				"File.mock.obj",
			});
			auto expectedInput = std::vector<Path>({
				Path("Library.mock.lib"),
				Path("File.mock.obj"),
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
