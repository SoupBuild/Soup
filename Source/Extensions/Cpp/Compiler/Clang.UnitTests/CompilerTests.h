// <copyright file="CompilerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler::Clang::UnitTests
{
	class CompilerTests
	{
	public:
		[[Fact]]
		void Initialize()
		{
			auto uut = Compiler(Path("C:/Clang/bin/"));
			Assert::AreEqual(uut.GetName(), std::string_view("Clang"), "Verify name match expected.");
			Assert::AreEqual(uut.GetObjectFileExtension(), std::string_view("obj"), "Verify object file extension match expected.");
			Assert::AreEqual(uut.GetModuleFileExtension(), std::string_view("pcm"), "Verify module file extension match expected.");
			Assert::AreEqual(uut.GetStaticLibraryFileExtension(), std::string_view("lib"), "Verify static library file extension match expected.");
			Assert::AreEqual(uut.GetDynamicLibraryFileExtension(), std::string_view("dll"), "Verify dynamic library file extension match expected.");
		}

		[[Fact]]
		void Compile_Simple()
		{
			auto uut = Compiler(Path("C:/Clang/bin/"));

			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("obj/File.o");
			arguments.RootDirectory = Path("Source");

			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.CreateCompileOperation(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::Utilities::BuildOperation>(
				new Build::Utilities::BuildOperation(
					"./File.cpp",
					Path("C:/Clang/bin/clang++.exe"),
					"-nostdinc -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11 -c ./File.cpp -o ./obj/File.o",
					Path("Source"),
					std::vector<Path>({
						Path("File.cpp"),
					}),
					std::vector<Path>({
						Path("obj/File.o"),
					})));

			AssertExtensions::AreEqual(expected, result);
		}

		[[Fact]]
		void Compile_Module()
		{
			auto uut = Compiler(Path("C:/Clang/bin/"));

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

			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.CreateCompileOperation(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::Utilities::BuildOperation>(
				new Build::Utilities::BuildOperation(
					"./File.cpp",
					Path("C:/Clang/bin/clang++.exe"),
					"-nostdinc -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11 -I\"./Includes\" -DDEBUG -fmodule-file=\"./Module.pcm\" --precompile ./File.cpp -o ./obj/File.pcm",
					Path("Source"),
					std::vector<Path>({
						Path("Module.pcm"),
						Path("File.cpp"),
					}),
					std::vector<Path>({
						Path("obj/File.pcm"),
					}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						new Build::Utilities::BuildOperation(
							"./obj/File.pcm",
							Path("C:/Clang/bin/clang++.exe"),
							"-nostdinc -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11 -c ./obj/File.pcm -o ./obj/File.obj",
							Path("Source"),
							std::vector<Path>({
								Path("obj/File.pcm"),
							}),
							std::vector<Path>({
								Path("obj/File.obj"),
							})),
					})));

			AssertExtensions::AreEqual(expected, result);
		}

		[[Fact]]
		void LinkStaticLibrary_Simple()
		{
			auto uut = Compiler(Path("C:/Clang/bin/"));

			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.a");
			arguments.RootDirectory = Path("Source");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.CreateLinkOperation(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::Utilities::BuildOperation>(
				new Build::Utilities::BuildOperation(
					"./Library.mock.a",
					Path("C:/Clang/bin/llvm-ar.exe"),
					"rc ./Library.mock.a ./File.mock.o",
					Path("Source"),
					std::vector<Path>({
						Path("File.mock.o"),
					}),
					std::vector<Path>({
						Path("Library.mock.a"),
					})));

			AssertExtensions::AreEqual(expected, result);
		}

		[[Fact]]
		void LinkExecutable_Simple()
		{
			auto uut = Compiler(Path("C:/Clang/bin/"));

			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::Executable;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("Something.exe");
			arguments.RootDirectory = Path("Source");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.a"),
			});

			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.CreateLinkOperation(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::Utilities::BuildOperation>(
				new Build::Utilities::BuildOperation(
					"./Something.exe",
					Path("C:/Clang/bin/lld-link.exe"),
					"/nologo /subsystem:console /machine:X64 /out:\"./Something.exe\" ./Library.mock.a ./File.mock.o",
					Path("Source"),
					std::vector<Path>({
						Path("Library.mock.a"),
						Path("File.mock.o"),
					}),
					std::vector<Path>({
						Path("Something.exe"),
					})));

			AssertExtensions::AreEqual(expected, result);
		}
	};
}
