// <copyright file="LinkerArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler::Clang::UnitTests
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

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"rc",
				"./Library.mock.a",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
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
				auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);
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

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"rc",
				"./Library.mock.a",
				"./File.mock.o",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void DynamicLibrary()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::DynamicLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("Library.mock.so");
			arguments.ImplementationFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/subsystem:console",
				"/dll",
				"/implib:\"./Library.mock.lib\"",
				"/machine:X64",
				"/out:\"./Library.mock.so\"",
				"./File.mock.o",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void Executable()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::Executable;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("out/Something.exe");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.a"),
			});

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/subsystem:console",
				"/machine:X64",
				"/out:\"./out/Something.exe\"",
				"./Library.mock.a",
				"./File.mock.o",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}
	};
}
