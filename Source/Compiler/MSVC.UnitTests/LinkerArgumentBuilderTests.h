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
		void ZeroObjectFiles_Throws()
		{
			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.lib");
			arguments.ObjectFiles = std::vector<Path>({});
			Assert::ThrowsRuntimeError([&arguments]() {
				auto actual = ArgumentBuilder::BuildLinkerArguments(arguments);
			});
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
				auto actual = ArgumentBuilder::BuildLinkerArguments(arguments);
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
			auto actual = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/OUT:\"Library.mock.lib\"",
				"File.mock.o",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
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
			auto actual = ArgumentBuilder::BuildLinkerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/NOLOGO",
				"/OUT:\"out/Something.exe\"",
				"Library.mock.lib",
				"File.mock.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}
	};
}
