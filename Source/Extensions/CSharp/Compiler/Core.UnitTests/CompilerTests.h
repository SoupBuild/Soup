// <copyright file="CompilerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp::Compiler::UnitTests
{
	class CompilerTests
	{
	public:
		[[Fact]]
		void Initialize()
		{
			auto uut = Compiler(
				Path("./bin/"),
				Path("csc.test.exe"));
		}

		[[Fact]]
		void Compile_Library_Simple()
		{
			auto uut = Compiler(
				Path("./bin/"),
				Path("csc.test.exe"));

			CompileArguments arguments = {};
			arguments.TargetType = TargetType::Library;
			arguments.TargetFile = Path("Library.mock.dll");
			arguments.RootDirectory = Path("Source");
			arguments.SourceFiles = std::vector<Path>({
				Path("File.mock.cs"),
			});

			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.CreateCompileOperation(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::Utilities::BuildOperation>(
				new Build::Utilities::BuildOperation(
					"CoreCompile",
					Path("./bin/csc.test.exe"),
					"/noconfig /unsafe- /checked- /fullpaths /nostdlib+ /errorreport:prompt /warn:4 /errorendlocation /preferreduilang:en-US /highentropyva+ /debug- /debug:portable /filealign:512 /optimize- /out:./Library.mock.dll /target:library /warnaserror+ /utf8output /deterministic+ /langversion:8.0 ./File.mock.cs",
					Path("Source"),
					std::vector<Path>({
					}),
					std::vector<Path>({
					})));

			AssertExtensions::AreEqual(expected, result);
		}

		[[Fact]]
		void Compile_Executable_Simple()
		{
			auto uut = Compiler(
				Path("./bin/"),
				Path("csc.test.exe"));

			CompileArguments arguments = {};
			arguments.TargetType = TargetType::Executable;
			arguments.TargetFile = Path("Something.exe");
			arguments.RootDirectory = Path("Source");
			arguments.SourceFiles = std::vector<Path>({
				Path("File.mock.cs"),
			});
			arguments.LibraryFiles = std::vector<Path>({
				Path("Library.mock.dll"),
			});

			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.CreateCompileOperation(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify result
			auto expected = Memory::Reference<Build::Utilities::BuildOperation>(
				new Build::Utilities::BuildOperation(
					"CoreCompile",
					Path("./bin/csc.test.exe"),
					"/noconfig /unsafe- /checked- /fullpaths /nostdlib+ /errorreport:prompt /warn:4 /errorendlocation /preferreduilang:en-US /highentropyva+ /reference:./Library.mock.dll /debug- /debug:portable /filealign:512 /optimize- /out:./Something.exe /target:exe /warnaserror+ /utf8output /deterministic+ /langversion:8.0 ./File.mock.cs",
					Path("Source"),
					std::vector<Path>({
					}),
					std::vector<Path>({
					})));
				
			AssertExtensions::AreEqual(expected, result);
		}
	};
}
