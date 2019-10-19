// <copyright file="CompilerArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::MSVC::UnitTests
{
	class CompilerArgumentBuilderTests
	{
	public:
		[[Fact]]
		void EmptySourceFile_Throws()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("");
			arguments.TargetFile = Path("File.obj");
			Assert::ThrowsRuntimeError([&arguments]() {
				auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);
			});
		}

		[[Fact]]
		void EmptyTargetFile_Throws()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("");
			Assert::ThrowsRuntimeError([&arguments]() {
				auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);
			});
		}

		[[Fact]]
		void DefaultParameters()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++11",
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void SingleArgument_GenerateIncludeTree()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.GenerateIncludeTree = true;
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++11",
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Theory]]
		[[InlineData(Soup::LanguageStandard::CPP11, "/std:c++11")]]
		[[InlineData(Soup::LanguageStandard::CPP14, "/std:c++14")]]
		[[InlineData(Soup::LanguageStandard::CPP17, "/clang:-std=c++17")]]
		void SingleArgument_LanguageStandard(LanguageStandard standard, std::string expectedFlag)
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.Standard = standard;
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				expectedFlag,
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void SingleArgument_LanguageStandard_CPP20()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.Standard = LanguageStandard::CPP20;
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/clang:-std=c++2a",
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void SingleArgument_OptimizationLevel_Disabled()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = OptimizationLevel::None;
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++17",
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Theory]]
		[[InlineData(Soup::OptimizationLevel::Size, "/Os")]]
		[[InlineData(Soup::OptimizationLevel::Speed, "/Ot")]]
		void SingleArgument_OptimizationLevel(OptimizationLevel level, std::string expectedFlag)
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = level;
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++17",
				expectedFlag,
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void SingleArgument_GenerateDebugInformation()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.Standard = LanguageStandard::CPP17;
			arguments.Optimize = OptimizationLevel::None;
			arguments.GenerateSourceDebugInfo = true;
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++17",
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void SingleArgument_IncludePaths()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.IncludeDirectories = std::vector<Path>({
				Path("C:/Files/SDK/"),
				Path("my files/")
			});
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++11",
				"/I\"C:/Files/SDK/\"",
				"/I\"my files/\"",
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void SingleArgument_PreprocessorDefinitions()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.PreprocessorDefinitions = std::vector<std::string>({
				"DEBUG",
				"VERSION=1"
			});
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++11",
				"/DDEBUG",
				"/DVERSION=1",
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void SingleArgument_Modules()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("File.cpp");
			arguments.TargetFile = Path("File.obj");
			arguments.IncludeModules = std::vector<Path>({
				Path("Module.pcm"),
				Path("Std.pcm")
			});
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++11",
				"/c",
				"File.cpp",
				"/o",
				"File.obj",
			});

			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}

		[[Fact]]
		void SingleArgument_ExportModule_SingleSource()
		{
			CompileArguments arguments = {};
			arguments.SourceFile = Path("module.cpp");
			arguments.TargetFile = Path("module.pcm");
			arguments.ExportModule = true;
			auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expected = std::vector<std::string>({
				"/std:c++11",
				"/clang:--precompile",
				"module.cpp",
				"/o",
				"module.pcm",
			});
			Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
		}
	};
}
