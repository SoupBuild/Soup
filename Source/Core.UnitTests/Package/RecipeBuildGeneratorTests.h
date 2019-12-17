// <copyright file="RecipeBuildGeneratorTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class RecipeBuildGeneratorTests
	{
	public:
		[[Fact]]
		void Initialize_Success()
		{
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = RecipeBuildGenerator(compiler);
		}

		[[Fact]]
		void Initialize_NullCompilerThrows()
		{
			Assert::ThrowsRuntimeError([]() {
				auto uut = RecipeBuildGenerator(nullptr);
			});
		}

		[[Fact]]
		void EnsureExecutableBuilt()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			// Register the mock Soup.Core dependency
			fileSystem->CreateMockFile(
				Path("D:/Repos/Soup/Source/Core/Recipe.json"),
				MockFileState(std::stringstream(R"({
					"name": "Soup.Core",
					"version": "1.2.3"
				})")));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = RecipeBuildGenerator(compiler);

			auto workingDirectory = Path("Root");
			auto recipe = Recipe();
			uut.EnsureExecutableBuilt(workingDirectory, recipe);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Create Directory: Root/.soup/Build",
					"INFO: Generate Build File: Build.cpp",
					"INFO: Compiling Build Executable",
					"DIAG: TargetName = Soup.RecipeBuild",
					"DIAG: TargetType = Executable",
					"DIAG: WorkingDirectory = Root/.soup/Build",
					"DIAG: ObjectDirectory = out/obj/MockCompiler",
					"DIAG: BinaryDirectory = out/bin/MockCompiler",
					"DIAG: ModuleInterfaceSourceFile = ",
					"DIAG: IsIncremental = true",
					"DIAG: IncludeDirectories = ",
					"DIAG: IncludeModules = D:/Repos/Soup/Source/Core/out/bin/MockCompiler/Soup.Core.mock.bmi",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Task: CoreCompile",
					"INFO: Loading previous build state",
					"INFO: BuildState file does not exist",
					"INFO: No previous state found, full rebuild required",
					"INFO: Create Directory: out/obj/MockCompiler",
					"INFO: Create Directory: out/bin/MockCompiler",
					"INFO: Task: CompileSourceFiles",
					"INFO: Compiling source files",
					"INFO: Build.cpp",
					"INFO: Saving updated build state",
					"INFO: Create Directory: .soup",
					"INFO: Task: CoreLink",
					"INFO: Linking target",
					"INFO: out/bin/MockCompiler/Soup.RecipeBuild.exe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: Root/.soup/Build",
					"CreateDirectory: Root/.soup/Build",
					"OpenWrite: Root/.soup/Build/Build.cpp",
					"Exists: D:/Repos/Soup/Source/Core/Recipe.json",
					"OpenRead: D:/Repos/Soup/Source/Core/Recipe.json",
					"Exists: Root/.soup/Build/.soup/BuildState.json",
					"Exists: Root/.soup/Build/out/obj/MockCompiler",
					"CreateDirectory: Root/.soup/Build/out/obj/MockCompiler",
					"Exists: Root/.soup/Build/out/bin/MockCompiler",
					"CreateDirectory: Root/.soup/Build/out/bin/MockCompiler",
					"Exists: Root/.soup/Build/.soup",
					"CreateDirectory: Root/.soup/Build/.soup",
					"OpenWrite: Root/.soup/Build/.soup/BuildState.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify the contents of the build file
			std::string expectedBuildFile = 
R"(import Soup.Core;

int main()
{
})";
			auto& mockBuildFile = fileSystem->GetMockFile(Path("Root/.soup/Build/Build.cpp"));
			Assert::AreEqual(expectedBuildFile, mockBuildFile.Contents->str(), "Verify build file contents.");
		}

		// TODO: Way more of this
	};
}
