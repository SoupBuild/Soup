// <copyright file="LinkerArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler::MSVC::UnitTests
{
	class LinkerArgumentBuilderTests
	{
	public:
		// [[Fact]]
		void ZeroObjectFiles()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({});

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/machine:X64",
				"/out:\"./Library.mock.lib\"",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void EmptyTargetFile_Throws()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.obj"),
			});
			Assert::ThrowsRuntimeError([&arguments]() {
				auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);
			});
		}

		// [[Fact]]
		void StaticLibrary()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/machine:X64",
				"/out:\"./Library.mock.lib\"",
				"./File.mock.o",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void StaticLibrary_LibraryPaths()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});
			arguments.LibraryPaths = std::vector<Path>({
				Path("../libraries/"),
			});

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/machine:X64",
				"/libpath:\"../libraries/\"",
				"/out:\"./Library.mock.lib\"",
				"./File.mock.o",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void DynamicLibrary()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::DynamicLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("Library.mock.dll");
			arguments.ImplementationFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.obj"),
			});

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/subsystem:console",
				"/dll",
				"/implib:\"./Library.mock.lib\"",
				"/machine:X64",
				"/out:\"./Library.mock.dll\"",
				"./File.mock.obj",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void Executable()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::Executable;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("out/Something.exe");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.obj"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.lib"),
			});

			auto actualArguments = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/nologo",
				"/subsystem:console",
				"/machine:X64",
				"/out:\"./out/Something.exe\"",
				"./Library.mock.lib",
				"./File.mock.obj",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}
	};
}
