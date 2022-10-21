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
			auto arguments = RecipeBuildArguments();
			auto hostBuildGlobalParameters = ValueTable();
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);
		}

		// [[Fact]]
		void Load_NoDependencies_BuiltInLanguage()
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
					Language: "C++|1.2.3"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"ArgumentValue",
						Value(true),
					},
				}));
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"HostValue",
						Value(true),
					},
				}));
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

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
									std::map<std::string, Value>({
										{
											"ArgumentValue",
											Value(true),
										},
									})))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								Path("C:/WorkingDirectory/MyPackage/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}

		// [[Fact]]
		void Load_NoDependencies_ExternalLanguage()
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

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"ArgumentValue",
						Value(true),
					},
				}));
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"HostValue",
						Value(true),
					},
				}));
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

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
									std::map<std::string, Value>({
										{
											"ArgumentValue",
											Value(true),
										},
									})))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								Path("C:/WorkingDirectory/MyPackage/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
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
					Language: "C++|1"
					Dependencies: {
						Runtime: [
							"PackageA@1.2.3"
							"PackageB@1.1.1"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageA"
					Language: "C++|1"
					Dependencies: {
						Runtime: [
							"PackageB@1.1.1"
						]
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "PackageB"
					Language: "C++|1"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"ArgumentValue",
						Value(true),
					},
				}));
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"HostValue",
						Value(true),
					},
				}));
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"DIAG: Recipe closure already loaded: C++|PackageB",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					"GetCurrentDirectory",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

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
									std::map<std::string, Value>({
										{
											"ArgumentValue",
											Value(true),
										},
									})))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								Path("C:/WorkingDirectory/MyPackage/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
								PackageChildrenMap({
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference(std::nullopt, "PackageA", SemanticVersion(1, 2, 3)), false, 2, -1),
											PackageChildInfo(PackageReference(std::nullopt, "PackageB", SemanticVersion(1, 1, 1)), false, 3, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3"),
								recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
								PackageChildrenMap({
									{
										"Runtime",
										{
											PackageChildInfo(PackageReference(std::nullopt, "PackageB", SemanticVersion(1, 1, 1)), false, 3, -1),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1"),
								recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
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
					Language: "C++|1"
					Dependencies: {
						Build: [
							"TestBuild@1.2.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "C#|1"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"ArgumentValue",
						Value(true),
					},
				}));
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"HostValue",
						Value(true),
					},
				}));
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/C#/TestBuild/1.2.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"Exists: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/C#/TestBuild/1.2.3/PackageLock.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

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
									std::map<std::string, Value>({
										{
											"ArgumentValue",
											Value(true),
										},
									})))
						},
						{
							2,
							PackageGraph(
								2,
								2,
								ValueTable(
									std::map<std::string, Value>({
										{
											"HostValue",
											Value(true),
										},
									})))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								Path("C:/WorkingDirectory/MyPackage/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "TestBuild", SemanticVersion(1, 2, 3)), true, -1, 2),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3"),
								recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.CSharp/3.2.1/Soup.CSharp.dll"),
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
					Language: "C++|1"
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
					Language: "C#|1"
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"ArgumentValue",
						Value(true),
					},
				}));
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"HostValue",
						Value(true),
					},
				}));
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/WorkingDirectory/TestBuild/Recipe.sml",
					"DIAG: Load PackageLock: C:/WorkingDirectory/TestBuild/PackageLock.sml",
					"INFO: PackageLock file does not exist",
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
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

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
									std::map<std::string, Value>({
										{
											"ArgumentValue",
											Value(true),
										},
									})))
						},
						{
							2,
							PackageGraph(
								2,
								2,
								ValueTable(
									std::map<std::string, Value>({
										{
											"HostValue",
											Value(true),
										},
									})))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								Path("C:/WorkingDirectory/MyPackage/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(Path("../TestBuild/")), true, -1, 2),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								Path("C:/WorkingDirectory/TestBuild/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/TestBuild/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.CSharp/3.2.1/Soup.CSharp.dll"),
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
							"TestBuild@1.2.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "C#|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 3
					Closures: {
						Root: {
							Cpp: [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
							]
						}
						Build0: {
							CSharp: [
								{ Name: "Soup.Cpp", Version: "1.2.3" }
								{ Name: "TestBuild", Version: "1.2.3" }
							]
						}
					}
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"ArgumentValue",
						Value(true),
					},
				}));
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"HostValue",
						Value(true),
					},
				}));
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/C#/TestBuild/1.2.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
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
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/C#/TestBuild/1.2.3/PackageLock.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

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
									std::map<std::string, Value>({
										{
											"ArgumentValue",
											Value(true),
										},
									})))
						},
						{
							2,
							PackageGraph(
								2,
								2,
								ValueTable(
									std::map<std::string, Value>({
										{
											"HostValue",
											Value(true),
										},
									})))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								Path("C:/WorkingDirectory/MyPackage/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "TestBuild", SemanticVersion(1, 2, 3)), true, -1, 2),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3"),
								recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.CSharp/3.2.1/Soup.CSharp.dll"),
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
							"TestBuild@1.2.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "TestBuild"
					Language: "C#|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/Soup.CSharp/1.0.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.CSharp"
					Language: "C#|1"
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/Soup.Cpp/1.0.2/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Soup.Cpp"
					Language: "C#|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 3
					Closures: {
						Root: {
							Cpp: [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
							]
						}
						Build0: {
							CSharp: [
								{ Name: "Soup.Cpp", Version: "1.0.2" }
								{ Name: "TestBuild", Version: "1.3.0" }
							]
						}
					}
				)")));
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/locks/C#/TestBuild/1.3.0/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 3
					Closures: {
						Root: {
							CSharp: [
								{ Name: "TestBuild", Version: "../TestBuild/", Build: "Build0" }
							]
						}
						Build0: {
							CSharp: [
								{ Name: "Soup.CSharp", Version: "1.0.3" }
							]
						}
					}
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"ArgumentValue",
						Value(true),
					},
				}));
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"HostValue",
						Value(true),
					},
				}));
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/C#/TestBuild/1.3.0/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/Soup.CSharp/1.0.3/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/C#/Soup.CSharp/1.0.3/PackageLock.sml",
					"INFO: PackageLock file does not exist",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/Soup.Cpp/1.0.2/Recipe.sml",
					"DIAG: Load PackageLock: C:/Users/Me/.soup/locks/C#/Soup.Cpp/1.0.2/PackageLock.sml",
					"INFO: PackageLock file does not exist",
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
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/C#/TestBuild/1.3.0/PackageLock.sml",
					"OpenReadBinary: C:/Users/Me/.soup/locks/C#/TestBuild/1.3.0/PackageLock.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/Soup.CSharp/1.0.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/Soup.CSharp/1.0.3/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/C#/Soup.CSharp/1.0.3/PackageLock.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/Soup.Cpp/1.0.2/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/Soup.Cpp/1.0.2/Recipe.sml",
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/locks/C#/Soup.Cpp/1.0.2/PackageLock.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

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
									std::map<std::string, Value>({
										{
											"ArgumentValue",
											Value(true),
										},
									})))
						},
						{
							2,
							PackageGraph(
								2,
								3,
								ValueTable(
									std::map<std::string, Value>({
										{
											"HostValue",
											Value(true),
										},
									})))
						},
						{
							3,
							PackageGraph(
								3,
								2,
								ValueTable(
									std::map<std::string, Value>({
										{
											"HostValue",
											Value(true),
										},
									})))
						},
						{
							4,
							PackageGraph(
								4,
								4,
								ValueTable(
									std::map<std::string, Value>({
										{
											"HostValue",
											Value(true),
										},
									})))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								Path("C:/WorkingDirectory/MyPackage/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								std::nullopt,
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "TestBuild", SemanticVersion(1, 2, 3)), true, -1, 3),
											PackageChildInfo(PackageReference(std::nullopt, "Soup.Cpp", SemanticVersion(1, 0, 2)), true, -1, 4),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0"),
								recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml")),
								std::nullopt,
								PackageChildrenMap({
									{
										"Build",
										{
											PackageChildInfo(PackageReference(std::nullopt, "Soup.CSharp", SemanticVersion(1, 0, 3)), true, -1, 2),
										}
									},
								}))
						},
						{
							3,
							PackageInfo(
								3,
								Path("C:/Users/Me/.soup/packages/C#/Soup.CSharp/1.0.3"),
								recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C#/Soup.CSharp/1.0.3/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.CSharp/3.2.1/Soup.CSharp.dll"),
								PackageChildrenMap())
						},
						{
							4,
							PackageInfo(
								4,
								Path("C:/Users/Me/.soup/packages/C#/Soup.Cpp/1.0.2"),
								recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C#/Soup.Cpp/1.0.2/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.CSharp/3.2.1/Soup.CSharp.dll"),
								PackageChildrenMap())
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
							"C#|Package1@1.2.3"
						]
					}
				)")));

			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Name: "Package1"
					Language: "C#|1"
				)")));

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 3
					Closures: {
						Root: {
							Cpp: [
								{ Name: "MyPackage", Version: "../MyPackage/", Build: "Build0" }
							]
							CSharp: [
								{ Name: "Package1", Version: "1.2.3", Build: "Build1" }
							]
						}
						Build0: {
							CSharp: [
								{ Name: "Soup.Cpp", Version: "1.2.3" }
							]
						}
						Build1: {
							CSharp: [
								{ Name: "Soup.CSharp", Version: "3.2.1" }
							]
						}
					}
				)")));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.GlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"ArgumentValue",
						Value(true),
					},
				}));
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto hostBuildGlobalParameters = ValueTable(
				std::map<std::string, Value>({
					{
						"HostValue",
						Value(true),
					},
				}));
			auto recipeCache = RecipeCache();
			auto uut = BuildLoadEngine(
				SemanticVersion(1, 2, 3),
				SemanticVersion(3, 2, 1),
				arguments,
				hostBuildGlobalParameters,
				recipeCache);

			auto packageProvider = uut.Load();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: Load PackageLock: C:/WorkingDirectory/MyPackage/PackageLock.sml",
					"INFO: Package lock loaded",
					"DIAG: Load Recipe: C:/WorkingDirectory/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml",
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
					"GetCurrentDirectory",
					"Exists: C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

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
									std::map<std::string, Value>({
										{
											"ArgumentValue",
											Value(true),
										},
									})))
						},
					}),
					PackageLookupMap(
					{
						{
							1,
							PackageInfo(
								1,
								Path("C:/WorkingDirectory/MyPackage/"),
								recipeCache.GetRecipe(Path("C:/WorkingDirectory/MyPackage/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.Cpp/1.2.3/Soup.Cpp.dll"),
								PackageChildrenMap({
									{
										"Other",
										{
											PackageChildInfo(PackageReference("C#", "Package1", SemanticVersion(1, 2, 3)), false, 2, -1),
										}
									},
								}))
						},
						{
							2,
							PackageInfo(
								2,
								Path("C:/Users/Me/.soup/packages/C#/Package1/1.2.3"),
								recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml")),
								Path("C:/testlocation/Extensions/Soup.CSharp/3.2.1/Soup.CSharp.dll"),
								PackageChildrenMap())
						},
					})),
				packageProvider,
				"Verify package graph matches expected.");
		}
	};
}
