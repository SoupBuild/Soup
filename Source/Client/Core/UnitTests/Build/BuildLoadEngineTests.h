// <copyright file="BuildLoadEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class BuildLoadEngineTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>();
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>();
			auto targetBuildGlobalParameters = ValueTable();
			auto hostBuildGlobalParameters = ValueTable();
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);
		}

		// [[Fact]]
		void Load_LanguageExtension_BuiltInVersion()
		{
			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>();
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>();
			auto targetBuildGlobalParameters = ValueTable();
			auto hostBuildGlobalParameters = ValueTable();
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);
		}

		// [[Fact]]
		void Load_LanguageExtension_BuiltInVersion_ToolDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1.1.1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
					Dependencies: {
						Tool: [
							"C++|User1|TestTool@4.4.4"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/TestTool/4.4.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestTool"
					Language: "C++|1.1.1"
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"C#",
					KnownLanguage("CSharp", "User1", "Soup.CSharp")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.CSharp",
							SemanticVersion(2, 2, 2)
						},
					}
				},
				{
					"C++",
					{
						{
							"TestTool",
							SemanticVersion(4, 4, 4)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/TestTool/4.4.4/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/TestTool/4.4.4/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/TestTool/4.4.4/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								2,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								3,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(""),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 3),
										}
									},
									{
										"Tool",
										{
											PackageChildInfo(PackageReference("C++", "User1", "TestTool", SemanticVersion(4, 4, 4)), true, -1, 2),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "TestTool"),
								true,
								Path("C:/BuiltIn/Packages/TestTool/4.4.4/"),
								Path("C:/BuiltIn/Packages/TestTool/4.4.4/out/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_LanguageExtension_ExternalLanguage_ExactMatch()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1.1.1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"C#",
					KnownLanguage("CSharp", "User1", "Soup.CSharp")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.CSharp",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto arguments = RecipeBuildArguments();
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								2,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_LanguageExtension_PackageLock_ToolDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|4.5.6"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/4.5.6/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|2.2.2"
					Dependencies: {
						Tool: [
							"C++|User1|TestTool@4.4.4"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestTool"
					Language: "C++|1.1.1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|2.2.2"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|2.2.2"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							"C++": {
								MyPackage: { Version: "../MyPackage/", Build: "Build0", Tool: "Tool0" }
							}
						}
						Build0: {
							Wren: {
								"User1|Soup.Cpp": { Version: "4.5.6" }
							}
						}
						Tool0: {
							"C++": {
								"User1|TestTool": { Version: "3.3.3" }
							}
						}
					}
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"Wren",
					KnownLanguage("Wren", "User1", "Soup.Wren")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.Wren",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/4.5.6/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/Soup.Cpp/4.5.6/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Cpp/TestTool/3.3.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/4.5.6/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/4.5.6/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Wren/Soup.Cpp/4.5.6/PackageLock.sml",
					"Exists: C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Cpp/TestTool/3.3.3/PackageLock.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								3,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
						{
							4,
							PackageGraph(
								4,
								5,
								ValueTable())
						},
						{
							5,
							PackageGraph(
								5,
								2,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(4, 5, 6)), true, -1, 5),
										}
									},
									{
										"Tool",
										{
											PackageChildInfo(PackageReference("C++", "User1", "TestTool", SemanticVersion(3, 3, 3)), true, -1, 3),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "Soup.Cpp"),
								false,
								Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/4.5.6/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/4.5.6/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 4),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "TestTool"),
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							5,
							PackageInfo(
								5,
								PackageName("User1", "Soup.Wren"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/"),
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_TriangleDependency_NoRebuild()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1.1.1"
					Dependencies: {
						Runtime: [
							"User1|PackageA@3.3.3"
							"User1|PackageB@4.4.4"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageA"
					Language: "C++|1.1.1"
					Dependencies: {
						Runtime: [
							"User1|PackageB@4.4.4"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageB"
					Language: "C++|1.1.1"
				)")));
			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"C#",
					KnownLanguage("CSharp", "User1", "Soup.CSharp")
				},
				{
					"Wren",
					 KnownLanguage("Wren", "User1", "Soup.Wren")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.CSharp",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"DIAG: Graph already loaded: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/",
					"DIAG: Recipe already loaded: C++|User1|PackageB",
					"DIAG: Graph already loaded: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference("C++", "User1", "PackageA", SemanticVersion(3, 3, 3)), false, 2, -1),
											PackageChildInfo(PackageReference("C++", "User1", "PackageB", SemanticVersion(4, 4, 4)), false, 3, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "PackageA"),
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference("C++", "User1", "PackageB", SemanticVersion(4, 4, 4)), false, 3, -1),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "PackageB"),
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_NoPackageLock_ExternalReference()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1.1.1"
					Dependencies: {
						Runtime: [
							"User1|PackageA@3.3.3"
							"User1|PackageB@4.4.4"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageA"
					Language: "C++|1.1.1"
					Dependencies: {
						Runtime: [
							"User1|PackageB@4.4.4"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageB"
					Language: "C++|1.1.1"
				)")));
			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"C#",
					KnownLanguage("CSharp", "User1", "Soup.CSharp")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.CSharp",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"DIAG: Graph already loaded: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/",
					"DIAG: Recipe already loaded: C++|User1|PackageB",
					"DIAG: Graph already loaded: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference("C++", "User1", "PackageA", SemanticVersion(3, 3, 3)), false, 2, -1),
											PackageChildInfo(PackageReference("C++", "User1", "PackageB", SemanticVersion(4, 4, 4)), false, 3, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "PackageA"),
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/PackageA/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference("C++", "User1", "PackageB", SemanticVersion(4, 4, 4)), false, 3, -1),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "PackageB"),
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/PackageB/4.4.4/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_NoPackageLock_ExternalReference_ToolDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1.1.1"
					Dependencies: {
						Build: [
							"User1|TestBuild@3.3.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|2.2.2"
					Dependencies: {
						Tool: [
							"C++|User1|TestTool@4.4.4"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/TestTool/4.4.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestTool"
					Language: "C++|1.1.1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|1"
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"Wren",
					KnownLanguage("Wren", "User1", "Soup.Wren")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.Wren",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/TestTool/4.4.4/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Cpp/TestTool/4.4.4/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"DIAG: Graph already loaded: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
					"Exists: C:/Users/Me/.soup/packages/Cpp/TestTool/4.4.4/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/TestTool/4.4.4/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Cpp/TestTool/4.4.4/PackageLock.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								3,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
						{
							4,
							PackageGraph(
								4,
								5,
								ValueTable())
						},
						{
							5,
							PackageGraph(
								5,
								2,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "TestBuild", SemanticVersion(3, 3, 3)), true, -1, 5),
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
									{
										"Tool",
										{
											PackageChildInfo(PackageReference("C++", "User1", "TestTool", SemanticVersion(4, 4, 4)), true, -1, 3),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "TestBuild"),
								false,
								Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 4),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "TestTool"),
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/TestTool/4.4.4/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/TestTool/4.4.4/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							5,
							PackageInfo(
								5,
								PackageName("User1", "Soup.Wren"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/"),
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_NoPackageLock_Local()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1.1.1"
					Dependencies: {
						Build: [
							"../TestBuild/"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/TestBuild/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|2.2.2"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|1"
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"Wren",
					KnownLanguage("Wren", "User1", "Soup.Wren")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.Wren",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/WorkingDirectory/TestBuild/Recipe.sml",
					"DIAG: Load PackageLock: C:/WorkingDirectory/TestBuild/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/WorkingDirectory/TestBuild/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/TestBuild/Recipe.sml",
					"Exists: C:/WorkingDirectory/TestBuild/PackageLock.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								3,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								2,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
						{
							4,
							PackageGraph(
								4,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(Path("../TestBuild/")), true, -1, 3),
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 4),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "TestBuild"),
								false,
								Path("C:/WorkingDirectory/TestBuild/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/TestBuild/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "Soup.Wren"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/"),
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/out/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_PackageLock()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"User1|TestBuild@3.3.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|2.2.2"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							"C++": {
								MyPackage: { Version: "../MyPackage/", Build: "Build0", Tool: "Tool0" }
							}
						}
						Build0: {
							Wren: {
								"User1|Soup.Cpp": { Version: "1.1.1" }
								"User1|TestBuild": { Version: "3.3.3" }
							}
						}
						Tool0: {
						}
					}
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"Wren",
					KnownLanguage("Wren", "User1", "Soup.Wren")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.Wren",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{
										"ArgumentValue",
										Value(true),
									},
								}))
						},
						{
							2,
							PackageGraph(
								2,
								3,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								2,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
						{
							4,
							PackageGraph(
								4,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "TestBuild", SemanticVersion(3, 3, 3)), true, -1, 3),
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 4),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "TestBuild"),
								false,
								Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "Soup.Wren"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/"),
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/out/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_PackageLock_ToolDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"User1|TestBuild@3.3.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|2.2.2"
					Dependencies: {
						Tool: [
							"C++|User1|TestTool@4.4.4"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestTool"
					Language: "C++|1.1.1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							"C++": {
								MyPackage: { Version: "../MyPackage/", Build: "Build0", Tool: "Tool0" }
							}
						}
						Build0: {
							Wren: {
								"User1|Soup.Cpp": { Version: "1.1.1" }
								"User1|TestBuild": { Version: "3.3.3", Tool: "Tool0" }
							}
						}
						Tool0: {
							"C++": {
								"User1|TestTool": { Version: "3.3.3" }
							}
						}
					}
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"Wren",
					KnownLanguage("Wren", "User1", "Soup.Wren")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.Wren",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{ "ArgumentValue", Value(true) },
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{ "HostValue", Value(true) },
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Cpp/TestTool/3.3.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"DIAG: Graph already loaded: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.3/PackageLock.sml",
					"Exists: C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Cpp/TestTool/3.3.3/PackageLock.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{
										"ArgumentValue",
										Value(true),
									},
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								3,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
						{
							4,
							PackageGraph(
								4,
								5,
								ValueTable())
						},
						{
							5,
							PackageGraph(
								5,
								2,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "TestBuild", SemanticVersion(3, 3, 3)), true, -1, 5),
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
									{
										"Tool",
										{
											PackageChildInfo(PackageReference("C++", "User1", "TestTool", SemanticVersion(3, 3, 3)), true, -1, 3),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "TestBuild"),
								false,
								Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 4),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "TestTool"),
								false,
								Path("C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/"),
								Path(""),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Cpp/TestTool/3.3.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							5,
							PackageInfo(
								5,
								PackageName("User1", "Soup.Wren"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/"),
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_BuildDependency_PackageLock_Override()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Build: [
							"User1|TestBuild@3.3.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "Wren|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|2.2.2"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|2.2.2"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							"C++": {
								MyPackage: { Version: "../MyPackage/", Build: "Build0", Tool: "Tool0" }
							}
						}
						Build0: {
							Wren: {
								"User1|Soup.Cpp": { Version: "1.1.2" }
								"User1|TestBuild": { Version: "3.3.4" }
							}
						}
						Tool0: {
						}
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.4/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							Wren: {
								TestBuild: { Version: "../TestBuild/", Build: "Build0", Tool: "Tool0" }
							}
						}
						Build0: {
							Wren: {
								"User1|Soup.Wren": { Version: "2.2.3" }
							}
						}
						Tool0: {
						}
					}
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"Wren",
					KnownLanguage("Wren", "User1", "Soup.Wren")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.Wren",
							SemanticVersion(2, 2, 2)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{
					"ArgumentValue",
					Value(true),
				},
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{
					"HostValue",
					Value(true),
				},
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.4/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/Soup.Wren/2.2.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/Soup.Cpp/1.1.2/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Graph already loaded: C:/BuiltIn/Packages/Soup.Wren/2.2.2/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.4/PackageLock.sml",
					"OpenReadBinary: C:/Users/Me/.soup/locks/Wren/TestBuild/3.3.4/PackageLock.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Wren/Soup.Wren/2.2.3/PackageLock.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Wren/Soup.Cpp/1.1.2/PackageLock.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{
										"ArgumentValue",
										Value(true),
									},
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								3,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
						{
							4,
							PackageGraph(
								4,
								2,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
						{
							5,
							PackageGraph(
								5,
								5,
								ValueTable(
								{
									{
										"HostValue",
										Value(true),
									},
								}))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "TestBuild", SemanticVersion(3, 3, 4)), true, -1, 4),
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 2)), true, -1, 5),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "TestBuild"),
								false,
								Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/TestBuild/3.3.4/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 3)), true, -1, 3),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "Soup.Wren"),
								false,
								Path("C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/Soup.Wren/2.2.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Wren"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/"),
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/out/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							5,
							PackageInfo(
								5,
								PackageName("User1", "Soup.Cpp"),
								false,
								Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/Soup.Cpp/1.1.2/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_OtherDependency_PackageLock_ExplicitLanguage()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C++|1"
					Dependencies: {
						Other: [
							"C#|User1|Package1@4.4.4"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Package1"
					Language: "C#|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "Wren|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.CSharp/3.3.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.CSharp"
					Language: "Wren|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							"C++": {
								MyPackage: { Version: "../MyPackage/", Build: "Build0", Tool: "Tool0" }
							}
							"C#": {
								"User1|Package1": { Version: "4.4.4", Build: "Build1", Tool: "Tool0" }
							}
						}
						Build0: {
							Wren: {
								"User1|Soup.Cpp": { Version: "1.1.1" }
							}
						}
						Build1: {
							Wren: {
								"User1|Soup.CSharp": { Version: "3.3.3" }
							}
						}
						Tool1: {
						}
					}
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"C++",
					 KnownLanguage("Cpp", "User1", "Soup.Cpp")
				},
				{
					"Wren",
					KnownLanguage("Wren", "User1", "Soup.Wren")
				},
				{
					"C#",
					KnownLanguage("CSharp", "User1", "Soup.CSharp")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Cpp",
							SemanticVersion(1, 1, 1)
						},
						{
							"Soup.Wren",
							SemanticVersion(2, 2, 2)
						},
						{
							"Soup.CSharp",
							SemanticVersion(3, 3, 3)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{
					"ArgumentValue",
					Value(true),
				},
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{
					"HostValue",
					Value(true),
				},
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.CSharp/3.3.3/Recipe.sml",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.CSharp/3.3.3/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.CSharp/3.3.3/Recipe.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Cpp/1.1.1/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{
										"ArgumentValue",
										Value(true),
									},
								}))
						},
						{
							2,
							PackageGraph(
								2,
								3,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								4,
								ValueTable())
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Cpp", SemanticVersion(1, 1, 1)), true, -1, 3),
										}
									},
									{
										"Other",
										{
											PackageChildInfo(PackageReference("C#", "User1", "Package1", SemanticVersion(4, 4, 4)), false, 2, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "Package1"),
								false,
								Path("C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/CSharp/Package1/4.4.4/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.CSharp", SemanticVersion(3, 3, 3)), true, -1, 2),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "Soup.CSharp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.CSharp/3.3.3/"),
								Path("C:/BuiltIn/Packages/Soup.CSharp/3.3.3/out/"),
								nullptr,
								PackageChildrenMap())
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Cpp"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/"),
								Path("C:/BuiltIn/Packages/Soup.Cpp/1.1.1/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_RuntimeDependency_PackageLock_ExplicitLanguageOverride_MultipleReferences_ReuseBuildGraph()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			// Create the Recipe to build
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "MyPackage"
					Language: "C#|1"
					Dependencies: {
						Runtime: [
							"../Package1/"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/Package1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Package1"
					Language: "C#|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.CSharp"
					Language: "Wren|2.2.2"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Wren"
					Language: "Wren|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 5
					Closures: {
						Root: {
							"C#": {
								MyPackage: { Version: "../MyPackage/", Build: "Build0", Tool: "Tool0" }
								Package1: { Version: "../Package1/", Build: "Build0", Tool: "Tool0" }
							}
						}
						Build0: {
							Wren: {
								"User1|Soup.CSharp": { Version: "2.2.3" }
							}
						}
						Tool0: {
						}
					}
				)")));

			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto knownLanguages = std::map<std::string, KnownLanguage>(
			{
				{
					"Wren",
					KnownLanguage("Wren", "User1", "Soup.Wren")
				},
				{
					"C#",
					KnownLanguage("CSharp", "User1", "Soup.CSharp")
				},
			});
			auto builtInPackages = std::map<std::string, std::map<std::string, SemanticVersion>>(
			{
				{
					"Wren",
					{
						{
							"Soup.Wren",
							SemanticVersion(2, 2, 2)
						},
						{
							"Soup.CSharp",
							SemanticVersion(3, 3, 3)
						},
					}
				},
			});
			auto targetBuildGlobalParameters = ValueTable(
			{
				{
					"ArgumentValue",
					Value(true),
				},
			});
			auto hostBuildGlobalParameters = ValueTable(
			{
				{
					"HostValue",
					Value(true),
				},
			});
			auto userDataPath = Path("C:/Users/Me/.soup/");
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				builtInPackageDirectory,
				knownLanguages,
				builtInPackages,
				targetBuildGlobalParameters,
				hostBuildGlobalParameters,
				userDataPath,
				recipeCache);

			auto workingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto packageProvider = uut.Load(workingDirectory);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/WorkingDirectory/Package1/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/Wren/Soup.CSharp/2.2.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"DIAG: Graph already loaded: C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"Exists: C:/WorkingDirectory/Package1/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/Package1/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/locks/Wren/Soup.CSharp/2.2.3/PackageLock.sml",
					"Exists: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
					"OpenReadBinary: C:/BuiltIn/Packages/Soup.Wren/2.2.2/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected package graph
			Assert::AreEqual(
				PackageProvider(
					1,
					PackageGraphLookupMap(
					{
						{
							1,
							PackageGraph(
								1,
								1,
								ValueTable(
								{
									{ "ArgumentValue", Value(true) },
								}))
						},
						{
							2,
							PackageGraph(
								2,
								4,
								ValueTable())
						},
						{
							3,
							PackageGraph(
								3,
								3,
								ValueTable(
								{
									{ "HostValue", Value(true) },
								}))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								PackageName(std::nullopt, "MyPackage"),
								false,
								Path("C:/WorkingDirectory/MyPackage/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.CSharp", SemanticVersion(2, 2, 3)), true, -1, 3),
										}
									},
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference(Path("../Package1/")), false, 2, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								PackageName("User1", "Package1"),
								false,
								Path("C:/WorkingDirectory/Package1/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/WorkingDirectory/Package1/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.CSharp", SemanticVersion(2, 2, 3)), true, -1, 3),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								PackageName("User1", "Soup.CSharp"),
								false,
								Path("C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/"),
								Path(),
								&recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/Wren/Soup.CSharp/2.2.3/Recipe.sml")),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference("Wren", "User1", "Soup.Wren", SemanticVersion(2, 2, 2)), true, -1, 2),
										}
									},
								}))
						},
						{
							4,
							PackageInfo(
								4,
								PackageName("User1", "Soup.Wren"),
								true,
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/"),
								Path("C:/BuiltIn/Packages/Soup.Wren/2.2.2/out/"),
								nullptr,
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}
	};
}
