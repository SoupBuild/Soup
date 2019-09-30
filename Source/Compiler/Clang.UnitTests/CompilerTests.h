// <copyright file="CompilerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::Clang::UnitTests
{
	class CompilerTests
	{
	public:
		[[Fact]]
		void Initialize()
		{
			auto uut = Compiler();
			Assert::AreEqual(uut.GetName(), std::string_view("Clang"), "Verify name match expected.");
			Assert::AreEqual(uut.GetObjectFileExtension(), std::string_view("o"), "Verify object file extension match expected.");
			Assert::AreEqual(uut.GetModuleFileExtension(), std::string_view("pcm"), "Verify module file extension match expected.");
			Assert::AreEqual(uut.GetStaticLibraryFileExtension(), std::string_view("a"), "Verify static library file extension match expected.");
		}

		[[Fact]]
		void Compile_Simple()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			IProcessManager::Register(processManager);

			auto uut = Compiler();

			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("obj/File.o");
			arguments.RootDirectory = Path("Source");

			auto result = uut.Compile(arguments);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Source: C:/Clang/bin/clang++.exe -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11 -c File.cpp -o obj/File.o",
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

			auto uut = Compiler();

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
					"Source: C:/Clang/bin/clang++.exe -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11 -I\"Includes\" -DDEBUG -fmodule-file=\"Module.pcm\" --precompile File.cpp -o obj/File.pcm",
					"Source: C:/Clang/bin/clang++.exe -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11 -c obj/File.pcm -o obj/File.obj",
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

			auto uut = Compiler();

			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.a");
			arguments.RootDirectory = Path("Source");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			uut.Link(arguments);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Source: C:/Clang/bin/llvm-ar.exe rc Library.mock.a File.mock.o",
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

			auto uut = Compiler();

			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::Executable;
			arguments.TargetFile = Path("Something.exe");
			arguments.RootDirectory = Path("Source");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.a"),
			});

			uut.Link(arguments);

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Source: C:/Clang/bin/clang++.exe -o Something.exe Library.mock.a File.mock.o",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");
		}
	};
}
