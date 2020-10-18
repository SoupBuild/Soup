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

			SharedCompileArguments arguments = {};
			arguments.RootDirectory = Path("Source/");
			arguments.ObjectDirectory = Path("ObjectDir/");

			auto translationUnitArguments = TranslationUnitCompileArguments();
			translationUnitArguments.SourceFile = Path("File.cpp");
			translationUnitArguments.TargetFile = Path("obj/File.o");
			arguments.ImplementationUnits.push_back(translationUnitArguments);

			auto result = uut.CreateCompileOperations(arguments);

			// Verify result
			auto expected = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"WriteFile [./ObjectDir/SharedCompileArguments.txt]",
					Path("Source/"),
					Path("./writefile.exe"),
					"\"./ObjectDir/SharedCompileArguments.txt\" \"-nostdinc -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./ObjectDir/SharedCompileArguments.txt"),
					})),
				Build::Utilities::BuildOperation(
					"./File.cpp",
					Path("Source/"),
					Path("C:/Clang/bin/clang++.exe"),
					"@./ObjectDir/SharedCompileArguments.txt -c ./File.cpp -o ./obj/File.o",
					std::vector<Path>({
						Path("File.cpp"),
						Path("./ObjectDir/SharedCompileArguments.txt"),
					}),
					std::vector<Path>({
						Path("obj/File.o"),
					})),
				});

			Assert::AreEqual(
				expected,
				result,
				"Verify result matches expected.");
		}

		[[Fact]]
		void Compile_Module()
		{
			auto uut = Compiler(Path("C:/Clang/bin/"));

			SharedCompileArguments arguments = {};
			arguments.RootDirectory = Path("Source/");
			arguments.ObjectDirectory = Path("ObjectDir/");
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Includes"),
			});
			arguments.IncludeModules = std::vector<Path>({
				Path("Module.pcm"),
			});
			arguments.PreprocessorDefinitions = std::vector<std::string>({
				"DEBUG"
			});

			auto compileModuleArguments = InterfaceUnitCompileArguments();
			compileModuleArguments.SourceFile = Path("File.cpp");
			compileModuleArguments.TargetFile = Path("obj/File.obj");
			compileModuleArguments.ModuleInterfaceTarget = Path("obj/File.pcm");
			arguments.InterfaceUnit = compileModuleArguments;

			auto result = uut.CreateCompileOperations(arguments);

			// Verify result
			auto expected = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"WriteFile [./ObjectDir/SharedCompileArguments.txt]",
					Path("Source/"),
					Path("./writefile.exe"),
					"\"./ObjectDir/SharedCompileArguments.txt\" \"-nostdinc -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11 -I\"./Includes\" -DDEBUG -fmodule-file=\"./Module.pcm\"\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./ObjectDir/SharedCompileArguments.txt"),
					})),
				Build::Utilities::BuildOperation(
					"./File.cpp",
					Path("Source/"),
					Path("C:/Clang/bin/clang++.exe"),
					"@./ObjectDir/SharedCompileArguments.txt --precompile ./File.cpp -o ./obj/File.pcm",
					std::vector<Path>({
						Path("Module.pcm"),
						Path("File.cpp"),
						Path("./ObjectDir/SharedCompileArguments.txt"),
					}),
					std::vector<Path>({
						Path("obj/File.pcm"),
					})),
				Build::Utilities::BuildOperation(
					"./obj/File.pcm",
					Path("Source/"),
					Path("C:/Clang/bin/clang++.exe"),
					"@./ObjectDir/SharedCompileArguments.txt -c ./obj/File.pcm -o ./obj/File.obj",
					std::vector<Path>({
						Path("obj/File.pcm"),
						Path("./ObjectDir/SharedCompileArguments.txt"),
					}),
					std::vector<Path>({
						Path("obj/File.obj"),
					})),
				});

			Assert::AreEqual(
				expected,
				result,
				"Verify result matches expected.");
		}

		[[Fact]]
		void LinkStaticLibrary_Simple()
		{
			auto uut = Compiler(Path("C:/Clang/bin/"));

			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::StaticLibrary;
			arguments.TargetFile = Path("Library.mock.a");
			arguments.RootDirectory = Path("Source/");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});

			auto result = uut.CreateLinkOperation(arguments);

			// Verify result
			auto expected = Build::Utilities::BuildOperation(
				"./Library.mock.a",
				Path("Source/"),
				Path("C:/Clang/bin/llvm-ar.exe"),
				"rc ./Library.mock.a ./File.mock.o",
				std::vector<Path>({
					Path("File.mock.o"),
				}),
				std::vector<Path>({
					Path("Library.mock.a"),
				}));

			Assert::AreEqual(expected, result, "Verify result matches expected.");
		}

		[[Fact]]
		void LinkExecutable_Simple()
		{
			auto uut = Compiler(Path("C:/Clang/bin/"));

			LinkArguments arguments = {};
			arguments.TargetType = LinkTarget::Executable;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = Path("Something.exe");
			arguments.RootDirectory = Path("Source/");
			arguments.ObjectFiles = std::vector<Path>({
				Path("File.mock.o"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.a"),
			});

			auto result = uut.CreateLinkOperation(arguments);

			// Verify result
			auto expected = Build::Utilities::BuildOperation(
				"./Something.exe",
				Path("Source/"),
				Path("C:/Clang/bin/lld-link.exe"),
				"/nologo /subsystem:console /machine:X64 /out:\"./Something.exe\" ./Library.mock.a ./File.mock.o",
				std::vector<Path>({
					Path("Library.mock.a"),
					Path("File.mock.o"),
				}),
				std::vector<Path>({
					Path("Something.exe"),
				}));

			Assert::AreEqual(expected, result, "Verify result matches expected.");
		}
	};
}
