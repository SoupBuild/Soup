// <copyright file="BuildGenerateEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
{
	class BuildGenerateEngineTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto uut = BuildGenerateEngine();
		}

		// [[Fact]]
		void Generate_Simple()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test library manager
			auto libraryManager = std::make_shared<MockLibraryManager>();
			auto scopedLibraryManager = ScopedLibraryManagerRegister(libraryManager);

			// Initialize the Build Extension libraries
			auto soupCppExtensionLibrary = libraryManager->RegisterLibrary(Path("./Soup.Cpp.dll"));
			soupCppExtensionLibrary->RegisterFunction("RegisterBuildExtension", &EmptyRegisterBuildExtension);

			// Initialize the recipe
			fileSystem->CreateMockFile(
				Path("C:/test/package/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name="MyPackage"
					Language="C++"
				)")));

			// Initialize the parameter file
			auto paramtersTable = ValueTable(
				std::map<std::string, Value>({
					{
						"PackageDirectory",
						Value(std::string("C:/test/package/")),
					},
					{
						"TargetDirectory",
						Value(std::string("C:/test/output/")),
					},
					{
						"SoupTargetDirectory",
						Value(std::string("C:/test/output/.soup/")),
					},
				}));
			auto parametersTableContent = std::stringstream();
			ValueTableWriter::Serialize(paramtersTable, parametersTableContent);
			fileSystem->CreateMockFile(
				Path("C:/test/output/.soup/GenerateParameters.bvt"),
				std::make_shared<MockFile>(std::move(parametersTableContent)));

			// Setup the input build state
			auto uut = BuildGenerateEngine();

			// Generate the build
			auto soupTargetDirectory = Path("C:/test/output/.soup/");
			uut.Generate(soupTargetDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build generate start",
					"DIAG: Load Recipe: C:/test/package/Recipe.toml",
					"DIAG: Running Build Extension: ./Soup.Cpp.dll",
					"INFO: Build Extension Done",
					"INFO: Create Directory: C:/test/output/.soup/",
					"INFO: Create Directory: C:/test/output/.soup/",
					"DIAG: Build generate end",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/test/output/.soup/GenerateParameters.bvt",
					"OpenReadBinary: C:/test/output/.soup/GenerateParameters.bvt",
					"Exists: C:/test/package/Recipe.toml",
					"OpenReadBinary: C:/test/package/Recipe.toml",
					"Exists: C:/test/output/.soup/",
					"CreateDirectory: C:/test/output/.soup/",
					"OpenWriteBinary: C:/test/output/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/test/output/.soup/",
					"CreateDirectory: C:/test/output/.soup/",
					"OpenWriteBinary: C:/test/output/.soup/GenerateSharedState.bvt",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected library manager requests
			Assert::AreEqual(
				std::vector<std::string>({
					"LoadDynamicLibrary: ./Soup.Cpp.dll",
					"GetFunction: ./Soup.Cpp.dll [RegisterBuildExtension]",
				}),
				libraryManager->GetRequests(),
				"Verify library manager requests match expected.");
		}

		// [[Fact]]
		void Generate_SingleRuntimeDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Register the test library manager
			auto libraryManager = std::make_shared<MockLibraryManager>();
			auto scopedLibraryManager = ScopedLibraryManagerRegister(libraryManager);

			// Initialize the Build Extension libraries
			auto soupCppExtensionLibrary = libraryManager->RegisterLibrary(Path("./Soup.Cpp.dll"));
			soupCppExtensionLibrary->RegisterFunction("RegisterBuildExtension", &EmptyRegisterBuildExtension);

			// Initialize the recipe
			fileSystem->CreateMockFile(
				Path("C:/test/package/Recipe.toml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name="MyPackage"
					Language="C++"
					[Dependencies]
					Runtime = [
						"../OtherPackage/",
					]
				)")));

			// Initialize the parameter file
			auto paramtersTable = ValueTable(
				std::map<std::string, Value>({
					{
						"PackageDirectory",
						Value(std::string("C:/test/package/")),
					},
					{
						"TargetDirectory",
						Value(std::string("C:/test/output/MyPackage/")),
					},
					{
						"SoupTargetDirectory",
						Value(std::string("C:/test/output/MyPackage/.soup/")),
					},
					{
						"Dependencies",
						Value(ValueTable(std::map<std::string, Value>({
							{
								"Runtime",
								Value(ValueTable(std::map<std::string, Value>({
									{
										"OtherPackage",
										Value(ValueTable(std::map<std::string, Value>({
											{
												"SoupTargetDirectory",
												Value(std::string("C:/test/output/OtherPackage/.soup/")),
											},
										}))),
									},
								}))),
							},
						}))),
					},
				}));
			auto parametersTableContent = std::stringstream();
			ValueTableWriter::Serialize(paramtersTable, parametersTableContent);
			fileSystem->CreateMockFile(
				Path("C:/test/output/MyPackage/.soup/GenerateParameters.bvt"),
				std::make_shared<MockFile>(std::move(parametersTableContent)));

			// Initialize the OtherPackage shared state file
			auto otherPackageSharedStateTable = ValueTable(
				std::map<std::string, Value>({
				}));
			auto otherPackageSharedStateTableContent = std::stringstream();
			ValueTableWriter::Serialize(otherPackageSharedStateTable, otherPackageSharedStateTableContent);
			fileSystem->CreateMockFile(
				Path("C:/test/output/OtherPackage/.soup/GenerateSharedState.bvt"),
				std::make_shared<MockFile>(std::move(otherPackageSharedStateTableContent)));

			// Setup the input build state
			auto uut = BuildGenerateEngine();

			// Generate the build
			auto soupTargetDirectory = Path("C:/test/output/MyPackage/.soup/");
			uut.Generate(soupTargetDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Build generate start",
					"DIAG: Load Recipe: C:/test/package/Recipe.toml",
					"DIAG: Running Build Extension: ./Soup.Cpp.dll",
					"INFO: Build Extension Done",
					"INFO: Create Directory: C:/test/output/MyPackage/.soup/",
					"INFO: Create Directory: C:/test/output/MyPackage/.soup/",
					"DIAG: Build generate end",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/test/output/MyPackage/.soup/GenerateParameters.bvt",
					"OpenReadBinary: C:/test/output/MyPackage/.soup/GenerateParameters.bvt",
					"Exists: C:/test/package/Recipe.toml",
					"OpenReadBinary: C:/test/package/Recipe.toml",
					"Exists: C:/test/output/OtherPackage/.soup/GenerateSharedState.bvt",
					"OpenReadBinary: C:/test/output/OtherPackage/.soup/GenerateSharedState.bvt",
					"Exists: C:/test/output/MyPackage/.soup/",
					"CreateDirectory: C:/test/output/MyPackage/.soup/",
					"OpenWriteBinary: C:/test/output/MyPackage/.soup/GenerateEvaluateGraph.bog",
					"Exists: C:/test/output/MyPackage/.soup/",
					"CreateDirectory: C:/test/output/MyPackage/.soup/",
					"OpenWriteBinary: C:/test/output/MyPackage/.soup/GenerateSharedState.bvt",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected library manager requests
			Assert::AreEqual(
				std::vector<std::string>({
					"LoadDynamicLibrary: ./Soup.Cpp.dll",
					"GetFunction: ./Soup.Cpp.dll [RegisterBuildExtension]",
				}),
				libraryManager->GetRequests(),
				"Verify library manager requests match expected.");
		}

	private:
		static int EmptyRegisterBuildExtension(IBuildSystem& system)
		{
			return 0;
		}
	};
}
