// <copyright file="ArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Rust::Compiler::UnitTests
{
	class ArgumentBuilderTests
	{
	public:
		// [[Fact]]
		void DefaultParameters()
		{
			CompileArguments arguments = {};
			arguments.SourceFiles = {
				Path("File.cs"),
			};
			arguments.TargetFile = Path("File.obj");

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/noconfig",
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:4",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/debug-",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:./File.obj",
				"/target:library",
				"/warnaserror+",
				"/utf8output",
				"/deterministic+",
				"/langversion:8.0",
				"./File.cs",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void SingleArgument_OptimizationLevel_Disabled()
		{
			CompileArguments arguments = {};
			arguments.SourceFiles = {
				Path("File.cs"),
			};
			arguments.TargetFile = Path("File.obj");
			arguments.EnableOptimize = false;

			auto actualInput = std::vector<Path>();
			auto actualOutput = std::vector<Path>();
			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/noconfig",
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:4",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/debug-",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:./File.obj",
				"/target:library",
				"/warnaserror+",
				"/utf8output",
				"/deterministic+",
				"/langversion:8.0",
				"./File.cs",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void SingleArgument_OptimizationLevel_Enabled()
		{
			CompileArguments arguments = {};
			arguments.SourceFiles = {
				Path("File.cs"),
			};
			arguments.TargetFile = Path("File.obj");
			arguments.EnableOptimize = true;
			arguments.GenerateSourceDebugInfo = false;

			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/noconfig",
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:4",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/debug-",
				"/debug:portable",
				"/filealign:512",
				"/optimize+",
				"/out:./File.obj",
				"/target:library",
				"/warnaserror+",
				"/utf8output",
				"/deterministic+",
				"/langversion:8.0",
				"./File.cs",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void SingleArgument_GenerateDebugInformation()
		{
			CompileArguments arguments = {};
			arguments.SourceFiles = {
				Path("File.cs"),
			};
			arguments.TargetFile = Path("File.obj");
			arguments.EnableOptimize = false;
			arguments.GenerateSourceDebugInfo = true;

			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/noconfig",
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:4",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/debug+",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:./File.obj",
				"/target:library",
				"/warnaserror+",
				"/utf8output",
				"/deterministic+",
				"/langversion:8.0",
				"./File.cs",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}

		// [[Fact]]
		void SingleArgument_PreprocessorDefinitions()
		{
			CompileArguments arguments = {};
			arguments.SourceFiles = {
				Path("File.cs"),
			};
			arguments.TargetFile = Path("File.obj");
			arguments.PreprocessorDefinitions = std::vector<std::string>({
				"DEBUG",
				"VERSION=1"
			});

			auto actualArguments = ArgumentBuilder::BuildCompilerArguments(arguments);

			auto expectedArguments = std::vector<std::string>({
				"/noconfig",
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:4",
				"/define:DEBUG;VERSION=1",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/debug-",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:./File.obj",
				"/target:library",
				"/warnaserror+",
				"/utf8output",
				"/deterministic+",
				"/langversion:8.0",
				"./File.cs",
			});

			Assert::AreEqual(expectedArguments, actualArguments, "Verify generated arguments match expected.");
		}
	};
}
