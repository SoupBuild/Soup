// <copyright file="CompilerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::MSVC::UnitTests
{
	class CompilerTests
	{
	public:
		[[Fact]]
		void Initialize()
		{
			auto uut = Compiler(
				Path("./bin/"),
				Path("mock.cl.exe"),
				Path("mock.link.exe"),
				Path("mock.lib.exe"));
			Assert::AreEqual(uut.GetName(), std::string_view("MSVC"), "Verify name match expected.");
			Assert::AreEqual(uut.GetObjectFileExtension(), std::string_view("obj"), "Verify object file extension match expected.");
			Assert::AreEqual(uut.GetModuleFileExtension(), std::string_view("pcm"), "Verify module file extension match expected.");
			Assert::AreEqual(uut.GetStaticLibraryFileExtension(), std::string_view("lib"), "Verify static library file extension match expected.");
			Assert::AreEqual(uut.GetDynamicLibraryFileExtension(), std::string_view("dll"), "Verify dynamic library file extension match expected.");
		}

		[[Fact]]
		void Compile_Simple()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			IProcessManager::Register(processManager);

			auto uut = Compiler(
				Path("./bin/"),
				Path("mock.cl.exe"),
				Path("mock.link.exe"),
				Path("mock.lib.exe"));

			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("obj/File.obj");
			arguments.RootDirectory = Path("Source");

			auto result = uut.Compile(arguments);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Source: bin/mock.cl.exe /nologo /std:c++11 /EHsc /c File.cpp /Fo\"obj/File.obj\"",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}

		[[Fact]]
		void Compile_Module()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			IProcessManager::Register(processManager);

			auto uut = Compiler(
				Path("./bin/"),
				Path("mock.cl.exe"),
				Path("mock.link.exe"),
				Path("mock.lib.exe"));

			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("obj/File.obj");
			arguments.RootDirectory = Path("Source");
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Includes"),
			});
			arguments.IncludeModules = std::vector<Path>({
				Path("Module.pcm"),
			});
			arguments.PreprocessorDefinitions = std::vector<std::string>({
				"DEBUG"
			});
			arguments.ExportModule = true;

			auto result = uut.Compile(arguments);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Source: bin/mock.cl.exe /nologo /std:c++11 /I\"Includes\" /DDEBUG /EHsc /clang:--precompile File.cpp /Fo\"obj/File.pcm\"",
					"Source: bin/mock.cl.exe /nologo /std:c++11 /EHsc /c obj/File.pcm /Fo\"obj/File.obj\"",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}

		[[Fact]]
		void LinkStaticLibrary_Simple()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			IProcessManager::Register(processManager);

			auto uut = Compiler(
				Path("./bin/"),
				Path("mock.cl.exe"),
				Path("mock.link.exe"),
				Path("mock.lib.exe"));

			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.a");
			arguments.RootDirectory = Path("Source");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.obj"),
			});

			uut.Link(arguments);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Source: bin/mock.lib.exe /NOLOGO /OUT:\"Library.mock.a\" File.mock.obj",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}

		[[Fact]]
		void LinkExecutable_Simple()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			IProcessManager::Register(processManager);

			auto uut = Compiler(
				Path("./bin/"),
				Path("mock.cl.exe"),
				Path("mock.link.exe"),
				Path("mock.lib.exe"));

			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::Executable;
			arguments.TargetFile = Path("Something.exe");
			arguments.RootDirectory = Path("Source");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.obj"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.a"),
			});

			uut.Link(arguments);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Source: bin/mock.link.exe /NOLOGO /OUT:\"Something.exe\" Library.mock.a File.mock.obj",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}
	};
}
