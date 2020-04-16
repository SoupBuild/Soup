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
			Assert::AreEqual(uut.GetModuleFileExtension(), std::string_view("ifc"), "Verify module file extension match expected.");
			Assert::AreEqual(uut.GetStaticLibraryFileExtension(), std::string_view("lib"), "Verify static library file extension match expected.");
			Assert::AreEqual(uut.GetDynamicLibraryFileExtension(), std::string_view("dll"), "Verify dynamic library file extension match expected.");
		}

		[[Fact]]
		void Compile_Simple()
		{
			auto uut = Compiler(
				Path("./bin/"),
				Path("mock.cl.exe"),
				Path("mock.link.exe"),
				Path("mock.lib.exe"));

			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("obj/File.obj");
			arguments.RootDirectory = Path("Source");

			auto buildState = Build::BuildState(Build::ValueTable());
			auto result = uut.CreateCompileNode(Build::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::BuildGraphNode>(
				new Build::BuildGraphNode(
					"File.cpp",
					"bin/mock.cl.exe",
					"/nologo /Zc:__cplusplus /std:c++11 /Od /X /RTC1 /EHsc /MT /bigobj /c File.cpp /Fo\"obj/File.obj\"",
					"Source",
					std::vector<std::string>({
						"File.cpp",
					}),
					std::vector<std::string>({
						"obj/File.obj",
					})));

			AssertExtensions::AreEqual(expected, result);
		}

		[[Fact]]
		void Compile_Module()
		{
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

			auto buildState = Build::BuildState(Build::ValueTable());
			auto result = uut.CreateCompileNode(Build::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::BuildGraphNode>(
				new Build::BuildGraphNode(
					"File.cpp",
					"bin/mock.cl.exe",
					"/nologo /Zc:__cplusplus /std:c++11 /Od /I\"Includes\" /DDEBUG /X /RTC1 /EHsc /MT /module:reference \"Module.pcm\" /module:interface /module:output \"obj/File.ifc\" /bigobj /c File.cpp /Fo\"obj/File.obj\"",
					"Source",
					std::vector<std::string>({
						"Module.pcm",
						"File.cpp",
					}),
					std::vector<std::string>({
						"obj/File.ifc",
						"obj/File.obj",
					})));

			AssertExtensions::AreEqual(expected, result);
		}

		[[Fact]]
		void LinkStaticLibrary_Simple()
		{
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

			auto buildState = Build::BuildState(Build::ValueTable());
			auto result = uut.CreateLinkNode(Build::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::BuildGraphNode>(
				new Build::BuildGraphNode(
					"Library.mock.a",
					"bin/mock.lib.exe",
					"/nologo /machine:X64 /out:\"Library.mock.a\" File.mock.obj",
					"Source",
					std::vector<std::string>({
						"File.mock.obj",
					}),
					std::vector<std::string>({
						"Library.mock.a",
					})));

			AssertExtensions::AreEqual(expected, result);
		}

		[[Fact]]
		void LinkExecutable_Simple()
		{
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

			auto buildState = Build::BuildState(Build::ValueTable());
			auto result = uut.CreateLinkNode(Build::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::BuildGraphNode>(
				new Build::BuildGraphNode(
					"Something.exe",
					"bin/mock.link.exe",
					"/nologo /subsystem:console /machine:X64 /out:\"Something.exe\" Library.mock.a File.mock.obj",
					"Source",
					std::vector<std::string>({
						"Library.mock.a",
						"File.mock.obj",
					}),
					std::vector<std::string>({
						"Something.exe",
					})));
				
			AssertExtensions::AreEqual(expected, result);
		}
	};
}
