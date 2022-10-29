// <copyright file="RecipeBuildRunnerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "MockEvaluateEngine.h"

namespace Soup::Core::UnitTests
{
	class RecipeBuildRunnerTests
	{
	public:
		// [[Fact]]
		void Initialize_Success()
		{
			auto arguments = RecipeBuildArguments();
			auto sdkParameters = ValueList();
			auto sdkReadAccess = std::vector<Path>();
			auto systemReadAccess = std::vector<Path>();
			auto recipeCache = RecipeCache();
			auto packageProvider = PackageProvider(1, PackageGraphLookupMap(), PackageLookupMap());
			auto evaluateEngine = MockEvaluateEngine();
			auto fileSystemState = FileSystemState();
			auto uut = RecipeBuildRunner(
				std::move(arguments),
				std::move(sdkParameters),
				std::move(sdkReadAccess),
				std::move(systemReadAccess),
				recipeCache,
				packageProvider,
				evaluateEngine,
				fileSystemState);
		}

		// [[Fact]]
		void Execute_NoDependencies()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState();

			auto operationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto operationGraphFiles = std::set<FileId>();
			auto operationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(operationGraph, operationGraphFiles, fileSystemState, operationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(operationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto sdkParameters = ValueList();
			auto sdkReadAccess = std::vector<Path>({
				Path("C:/FakeSDK/"),
			});
			auto systemReadAccess = std::vector<Path>({
				Path("C:/FakeSystem/"),
			});
			auto recipeCache = RecipeCache({
				{ "C:/WorkingDirectory/MyPackage/Recipe.sml", Recipe("MyPackage", LanguageReference("C++", SemanticVersion(1))) },
			});
			auto packageProvider = PackageProvider(
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
							Path("C:/Extension/Language.Build.dll"),
							PackageChildrenMap())
					},
				}));
			auto evaluateEngine = MockEvaluateEngine();
			auto uut = RecipeBuildRunner(
				arguments,
				sdkParameters,
				sdkReadAccess,
				systemReadAccess,
				recipeCache,
				packageProvider,
				evaluateEngine,
				fileSystemState);
			uut.Execute();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: 1>Running Build: C++|MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Checking for existing Evaluate Operation Graph",
					"INFO: 1>Previous graph found",
					"INFO: 1>Checking for existing Evaluate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Check outdated parameters file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Parameters file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Check outdated read access file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Read Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Check outdated write access file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Write Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Checking for existing Generate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Loading new Evaluate Operation Graph",
					"DIAG: 1>Map previous operation graph observed results",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"INFO: 1>Saving updated build state",
					"INFO: 1>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
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

			// Verify expected evaluate requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Evaluate: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"Evaluate: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
				}),
				evaluateEngine.GetRequests(),
				"Verify evaluate requests match expected.");

			// Verify files
			auto myPackageGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "ArgumentValue", Value(true), },
					{ "Dependencies", Value(ValueTable()), },
					{ "LanguageExtensionPath", Value(std::string("C:/Extension/Language.Build.dll")), },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
				})),
				ValueTableReader::Deserialize(myPackageGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/FakeSDK/\nC:/WorkingDirectory/MyPackage/\nC:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				myPackageGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				myPackageGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor"));
			auto myPackageGenerateResults = OperationResultsReader::Deserialize(myPackageGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							GetMinMillisecondTime(),
							{},
							{})
					},
				}),
				myPackageGenerateResults,
				"Verify generate results content match expected.");
		}

		// [[Fact]]
		void Execute_BuildDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState();

			auto myProjectOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto myProjectOperationGraphFiles = std::set<FileId>();
			auto myProjectOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(myProjectOperationGraph, myProjectOperationGraphFiles, fileSystemState, myProjectOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(myProjectOperationGraphContent)));

			auto testBuildOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto testBuildOperationGraphFiles = std::set<FileId>();
			auto testBuildOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(testBuildOperationGraph, testBuildOperationGraphFiles, fileSystemState,  testBuildOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(testBuildOperationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto sdkParameters = ValueList();
			auto sdkReadAccess = std::vector<Path>({
				Path("C:/FakeSDK/"),
			});
			auto systemReadAccess = std::vector<Path>({
				Path("C:/FakeSystem/"),
			});
			auto recipeCache = RecipeCache({
				{
					"C:/WorkingDirectory/MyPackage/Recipe.sml",
					Recipe(
						"MyPackage",
						LanguageReference("C++", SemanticVersion(1)),
						SemanticVersion(1, 0, 0),
						std::nullopt,
						std::vector<PackageReference>({
							PackageReference(std::nullopt, "TestBuild", SemanticVersion(1, 2, 3)),
						}),
						std::nullopt)
				},
				{
					"C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/Recipe.sml",
					Recipe(
						"TestBuild",
						LanguageReference("C#", SemanticVersion(1)),
						SemanticVersion(1, 2, 3),
						std::nullopt,
						std::nullopt,
						std::nullopt)
				},
			});
			auto packageProvider = PackageProvider(
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
							Path("C:/Extension/Language.Build.dll"),
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
							Path("C:/Extension/Language.Build.dll"),
							PackageChildrenMap())
					},
				}));
			auto evaluateEngine = MockEvaluateEngine();
			auto uut = RecipeBuildRunner(
				arguments,
				sdkParameters,
				sdkReadAccess,
				systemReadAccess,
				recipeCache,
				packageProvider,
				evaluateEngine,
				fileSystemState);
			uut.Execute();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: 2>Running Build: C#|TestBuild",
					"INFO: 2>Build 'TestBuild'",
					"INFO: 2>Checking for existing Evaluate Operation Graph",
					"INFO: 2>Previous graph found",
					"INFO: 2>Checking for existing Evaluate Operation Results",
					"INFO: 2>Operation results file does not exist",
					"INFO: 2>No previous results found",
					"INFO: 2>Check outdated parameters file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateParameters.bvt",
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Parameters file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"INFO: 2>Check outdated read access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateReadAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Read Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"INFO: 2>Check outdated write access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateWriteAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Write Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"INFO: 2>Checking for existing Generate Operation Results",
					"INFO: 2>Operation results file does not exist",
					"INFO: 2>No previous results found",
					"INFO: 2>Loading new Evaluate Operation Graph",
					"DIAG: 2>Map previous operation graph observed results",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"INFO: 2>Saving updated build state",
					"INFO: 2>Done",
					"DIAG: 1>Running Build: C++|MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Checking for existing Evaluate Operation Graph",
					"INFO: 1>Previous graph found",
					"INFO: 1>Checking for existing Evaluate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Check outdated parameters file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Parameters file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Check outdated read access file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Read Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Check outdated write access file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Write Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Checking for existing Generate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Loading new Evaluate Operation Graph",
					"DIAG: 1>Map previous operation graph observed results",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"INFO: 1>Saving updated build state",
					"INFO: 1>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bor",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Generate.bor",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Generate.bor",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
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

			// Verify expected evaluate requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Evaluate: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"Evaluate: C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"Evaluate: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"Evaluate: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					}),
					evaluateEngine.GetRequests(),
					"Verify evaluate requests match expected.");

			// Verify files
			auto testBuildGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Dependencies", Value(ValueTable()), },
					{ "HostValue", Value(true) },
					{ "LanguageExtensionPath", Value(std::string("C:/Extension/Language.Build.dll")), },
					{ "PackageDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/")), },
				})),
				ValueTableReader::Deserialize(testBuildGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto testBuildGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/FakeSDK/\nC:/Users/Me/.soup/packages/C#/TestBuild/1.2.3\nC:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/\n",
				testBuildGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto testBuildGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/\n",
				testBuildGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto testBuildGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Generate.bor"));
			auto testBuildGenerateResults = OperationResultsReader::Deserialize(testBuildGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							GetMinMillisecondTime(),
							{},
							{})
					},
				}),
				testBuildGenerateResults,
				"Verify test build generate results content match expected.");

			auto myPackageGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "ArgumentValue", Value(true) },
					{ "Dependencies", Value(ValueTable(std::map<std::string, Value>({
						{ "Build", Value(ValueTable(std::map<std::string, Value>({
							{ "TestBuild", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("TestBuild@1.2.3")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/")), },
							}))), },
						}))), },
					}))), },
					{ "LanguageExtensionPath", Value(std::string("C:/Extension/Language.Build.dll")), },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
				})),
				ValueTableReader::Deserialize(myPackageGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/FakeSDK/\nC:/Users/Me/.soup/packages/C#/TestBuild/1.2.3/out/zDqRc65c9x3jySpevCCCyZ15fGs/\nC:/WorkingDirectory/MyPackage/\nC:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				myPackageGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				myPackageGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor"));
			auto myPackageGenerateResults = OperationResultsReader::Deserialize(myPackageGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							GetMinMillisecondTime(),
							{},
							{})
					},
				}),
				myPackageGenerateResults,
				"Verify my package generate results content match expected.");
		}

		// [[Fact]]
		void Execute_TriangleDependency_NoRebuild()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState();

			auto myProjectOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto myProjectOperationGraphFiles = std::set<FileId>();
			auto myProjectOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(myProjectOperationGraph, myProjectOperationGraphFiles, fileSystemState, myProjectOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(myProjectOperationGraphContent)));
			auto packageAOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto packageAOperationGraphFiles = std::set<FileId>();
			auto packageAOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(packageAOperationGraph, packageAOperationGraphFiles, fileSystemState, packageAOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(packageAOperationGraphContent)));
			auto packageBOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto packageBOperationGraphFiles = std::set<FileId>();
			auto packageBOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(packageBOperationGraph, packageBOperationGraphFiles, fileSystemState, packageBOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(packageBOperationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto sdkParameters = ValueList();
			auto sdkReadAccess = std::vector<Path>({
				Path("C:/FakeSDK/"),
			});
			auto systemReadAccess = std::vector<Path>({
				Path("C:/FakeSystem/"),
			});
			auto recipeCache = RecipeCache({
				{
					"C:/WorkingDirectory/MyPackage/Recipe.sml",
					Recipe(
						"MyPackage",
						LanguageReference("C++", SemanticVersion(1)),
						SemanticVersion(1, 0, 0),
						std::vector<PackageReference>({
							PackageReference(std::nullopt, "PackageA", SemanticVersion(1, 2, 3)),
							PackageReference(std::nullopt, "PackageB", SemanticVersion(1, 1, 1)),
						}),
						std::nullopt,
						std::nullopt)
				},
				{
					"C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/Recipe.sml",
					Recipe(
						"PackageA",
						LanguageReference("C++", SemanticVersion(1)),
						SemanticVersion(1, 2, 3),
						std::vector<PackageReference>({
							PackageReference(std::nullopt, "PackageB", SemanticVersion(1, 1, 1)),
						}),
						std::nullopt,
						std::nullopt)
				},
				{
					"C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/Recipe.sml",
					Recipe(
						"PackageB",
						LanguageReference("C++", SemanticVersion(1)),
						SemanticVersion(1, 1, 1),
						std::nullopt,
						std::nullopt,
						std::nullopt)
				},
			});
			auto packageProvider = PackageProvider(
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
							Path("C:/Extension/Language.Build.dll"),
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
							Path("C:/Extension/Language.Build.dll"),
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
							Path("C:/Extension/Language.Build.dll"),
							PackageChildrenMap())
					},
				}));
			auto evaluateEngine = MockEvaluateEngine();
			auto uut = RecipeBuildRunner(
				arguments,
				sdkParameters,
				sdkReadAccess,
				systemReadAccess,
				recipeCache,
				packageProvider,
				evaluateEngine,
				fileSystemState);
			uut.Execute();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: 3>Running Build: C++|PackageB",
					"INFO: 3>Build 'PackageB'",
					"INFO: 3>Checking for existing Evaluate Operation Graph",
					"INFO: 3>Previous graph found",
					"INFO: 3>Checking for existing Evaluate Operation Results",
					"INFO: 3>Operation results file does not exist",
					"INFO: 3>No previous results found",
					"INFO: 3>Check outdated parameters file: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"INFO: 3>Value Table file does not exist",
					"INFO: 3>Save Parameters file",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 3>Check outdated read access file: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"INFO: 3>Path list file does not exist",
					"INFO: 3>Save Read Access file",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 3>Check outdated write access file: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"INFO: 3>Path list file does not exist",
					"INFO: 3>Save Write Access file",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 3>Checking for existing Generate Operation Results",
					"INFO: 3>Operation results file does not exist",
					"INFO: 3>No previous results found",
					"INFO: 3>Loading new Evaluate Operation Graph",
					"DIAG: 3>Map previous operation graph observed results",
					"INFO: 3>Create Directory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"INFO: 3>Saving updated build state",
					"INFO: 3>Done",
					"DIAG: 2>Running Build: C++|PackageA",
					"INFO: 2>Build 'PackageA'",
					"INFO: 2>Checking for existing Evaluate Operation Graph",
					"INFO: 2>Previous graph found",
					"INFO: 2>Checking for existing Evaluate Operation Results",
					"INFO: 2>Operation results file does not exist",
					"INFO: 2>No previous results found",
					"INFO: 2>Check outdated parameters file: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Parameters file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 2>Check outdated read access file: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Read Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 2>Check outdated write access file: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Write Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 2>Checking for existing Generate Operation Results",
					"INFO: 2>Operation results file does not exist",
					"INFO: 2>No previous results found",
					"INFO: 2>Loading new Evaluate Operation Graph",
					"DIAG: 2>Map previous operation graph observed results",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"INFO: 2>Saving updated build state",
					"INFO: 2>Done",
					"DIAG: 3>Running Build: C++|PackageB",
					"DIAG: 3>Recipe already built: C++|PackageB",
					"DIAG: 1>Running Build: C++|MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Checking for existing Evaluate Operation Graph",
					"INFO: 1>Previous graph found",
					"INFO: 1>Checking for existing Evaluate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Check outdated parameters file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Parameters file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Check outdated read access file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Read Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Check outdated write access file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Write Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Checking for existing Generate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Loading new Evaluate Operation Graph",
					"DIAG: 1>Map previous operation graph observed results",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"INFO: 1>Saving updated build state",
					"INFO: 1>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
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

			// Verify expected evaluate requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Evaluate: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"Evaluate: C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"Evaluate: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"Evaluate: C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"Evaluate: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"Evaluate: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
				}),
				evaluateEngine.GetRequests(),
				"Verify evaluate requests match expected.");

			// Verify files
			auto packageAGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "ArgumentValue", Value(true) },
					{ "Dependencies", Value(ValueTable(std::map<std::string, Value>({
						{ "Runtime", Value(ValueTable(std::map<std::string, Value>({
							{ "PackageB", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("PackageB@1.1.1")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
							}))), },
						}))), },
					}))), },
					{ "LanguageExtensionPath", Value(std::string("C:/Extension/Language.Build.dll")), },
					{ "PackageDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
				})),
				ValueTableReader::Deserialize(packageAGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto packageAGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/FakeSDK/\nC:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/\nC:/Users/Me/.soup/packages/C++/PackageA/1.2.3\nC:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				packageAGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto packageAGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				packageAGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto packageAGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor"));
			auto packageAGenerateResults = OperationResultsReader::Deserialize(packageAGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							GetMinMillisecondTime(),
							{},
							{})
					},
				}),
				packageAGenerateResults,
				"Verify package A generate results content match expected.");

			auto packageBGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "ArgumentValue", Value(true), },
					{ "Dependencies", Value(ValueTable()), },
					{ "LanguageExtensionPath", Value(std::string("C:/Extension/Language.Build.dll")), },
					{ "PackageDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
				})),
				ValueTableReader::Deserialize(packageBGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto packageBGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/FakeSDK/\nC:/Users/Me/.soup/packages/C++/PackageB/1.1.1\nC:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				packageBGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto packageBGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				packageBGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto packageBGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor"));
			auto packageBGenerateResults = OperationResultsReader::Deserialize(packageBGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							GetMinMillisecondTime(),
							{},
							{})
					},
				}),
				packageBGenerateResults,
				"Verify package B generate results content match expected.");

			auto myPackageGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "ArgumentValue", Value(true), },
					{ "Dependencies", Value(ValueTable(std::map<std::string, Value>({
						{ "Runtime", Value(ValueTable(std::map<std::string, Value>({
							{ "PackageA", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("PackageA@1.2.3")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
							}))), },
							{ "PackageB", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("PackageB@1.1.1")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
							}))), },
						}))), },
					}))), },
					{ "LanguageExtensionPath", Value(std::string("C:/Extension/Language.Build.dll")), },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
				})),
				ValueTableReader::Deserialize(myPackageGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/FakeSDK/\nC:/Users/Me/.soup/packages/C++/PackageA/1.2.3/out/zxAcy-Et010fdZUKLgFemwwWuC8/\nC:/Users/Me/.soup/packages/C++/PackageB/1.1.1/out/zxAcy-Et010fdZUKLgFemwwWuC8/\nC:/WorkingDirectory/MyPackage/\nC:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				myPackageGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				myPackageGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor"));
			auto myPackageGenerateResults = OperationResultsReader::Deserialize(myPackageGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							GetMinMillisecondTime(),
							{},
							{})
					},
				}),
				myPackageGenerateResults,
				"Verify my package generate results content match expected.");
		}

		// [[Fact]]
		void Execute_PackageLock_OverrideBuildDependency()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);
			auto fileSystemState = FileSystemState();

			// Create the package lock
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
				std::make_shared<MockFile>(std::stringstream(R"(
					Version: 3
					Closures: {
						Root: {
							CSharp: [
								{ Name: "TestBuild", Version: "1.3.0", Build: "Build1" }
							]
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
						Build1: {
							CSharp: [
								{ Name: "Soup.CSharp", Version: "1.0.1" }
							]
						}
					}
				)")));

			auto myProjectOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto myProjectOperationGraphFiles = std::set<FileId>();
			auto myProjectOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(myProjectOperationGraph, myProjectOperationGraphFiles, fileSystemState, myProjectOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(myProjectOperationGraphContent)));

			auto testBuildOperationGraph = OperationGraph(
				std::vector<OperationId>(),
				std::vector<OperationInfo>());
			auto testBuildOperationGraphFiles = std::set<FileId>();
			auto testBuildOperationGraphContent = std::stringstream();
			OperationGraphWriter::Serialize(testBuildOperationGraph, testBuildOperationGraphFiles, fileSystemState, testBuildOperationGraphContent);
			fileSystem->CreateMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog"),
				std::make_shared<MockFile>(std::move(testBuildOperationGraphContent)));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto arguments = RecipeBuildArguments();
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");
			auto sdkParameters = ValueList();
			auto sdkReadAccess = std::vector<Path>({
				Path("C:/FakeSDK/"),
			});
			auto systemReadAccess = std::vector<Path>({
				Path("C:/FakeSystem/"),
			});
			auto recipeCache = RecipeCache({
				{
					"C:/WorkingDirectory/MyPackage/Recipe.sml",
					Recipe(
						"MyPackage",
						LanguageReference("C++", SemanticVersion(1)),
						SemanticVersion(1, 0, 0),
						std::nullopt,
						std::vector<PackageReference>({
							PackageReference(std::nullopt, "TestBuild", SemanticVersion(1, 2, 3)),
						}),
						std::nullopt)
				},
				{
					"C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml",
					Recipe(
						"TestBuild",
						LanguageReference("C#", SemanticVersion(1)),
						SemanticVersion(1, 3, 0),
						std::nullopt,
						std::nullopt,
						std::nullopt)
				},
			});
			auto packageProvider = PackageProvider(
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
							Path("C:/Extension/Language.Build.dll"),
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
							Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0"),
							recipeCache.GetRecipe(Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/Recipe.sml")),
							Path("C:/Extension/Language.Build.dll"),
							PackageChildrenMap())
					},
				}));
			auto evaluateEngine = MockEvaluateEngine();
			auto uut = RecipeBuildRunner(
				arguments,
				sdkParameters,
				sdkReadAccess,
				systemReadAccess,
				recipeCache,
				packageProvider,
				evaluateEngine,
				fileSystemState);
			uut.Execute();

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: 2>Running Build: C#|TestBuild",
					"INFO: 2>Build 'TestBuild'",
					"INFO: 2>Checking for existing Evaluate Operation Graph",
					"INFO: 2>Previous graph found",
					"INFO: 2>Checking for existing Evaluate Operation Results",
					"INFO: 2>Operation results file does not exist",
					"INFO: 2>No previous results found",
					"INFO: 2>Check outdated parameters file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateParameters.bvt",
					"INFO: 2>Value Table file does not exist",
					"INFO: 2>Save Parameters file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"INFO: 2>Check outdated read access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateReadAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Read Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"INFO: 2>Check outdated write access file: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateWriteAccess.txt",
					"INFO: 2>Path list file does not exist",
					"INFO: 2>Save Write Access file",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"INFO: 2>Checking for existing Generate Operation Results",
					"INFO: 2>Operation results file does not exist",
					"INFO: 2>No previous results found",
					"INFO: 2>Loading new Evaluate Operation Graph",
					"DIAG: 2>Map previous operation graph observed results",
					"INFO: 2>Create Directory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"INFO: 2>Saving updated build state",
					"INFO: 2>Done",
					"DIAG: 1>Running Build: C++|MyPackage",
					"INFO: 1>Build 'MyPackage'",
					"INFO: 1>Checking for existing Evaluate Operation Graph",
					"INFO: 1>Previous graph found",
					"INFO: 1>Checking for existing Evaluate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Check outdated parameters file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"INFO: 1>Value Table file does not exist",
					"INFO: 1>Save Parameters file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Check outdated read access file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Read Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Check outdated write access file: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"INFO: 1>Path list file does not exist",
					"INFO: 1>Save Write Access file",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"INFO: 1>Checking for existing Generate Operation Results",
					"INFO: 1>Operation results file does not exist",
					"INFO: 1>No previous results found",
					"INFO: 1>Loading new Evaluate Operation Graph",
					"DIAG: 1>Map previous operation graph observed results",
					"INFO: 1>Create Directory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"INFO: 1>Saving updated build state",
					"INFO: 1>Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/RootRecipe.sml",
					"Exists: C:/Users/Me/RootRecipe.sml",
					"Exists: C:/Users/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bor",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateParameters.bvt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateReadAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateWriteAccess.txt",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Generate.bor",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Generate.bor",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog",
					"OpenReadBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bog",
					"Exists: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"OpenWriteBinary: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/RootRecipe.sml",
					"Exists: C:/RootRecipe.sml",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"OpenReadBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bog",
					"Exists: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"CreateDirectory: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"OpenWriteBinary: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Evaluate.bor",
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

			// Verify expected evaluate requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Evaluate: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"Evaluate: C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/temp/",
					"Evaluate: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
					"Evaluate: C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/temp/",
				}),
				evaluateEngine.GetRequests(),
				"Verify evaluate requests match expected.");

			// Verify files
			auto testBuildGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "Dependencies", Value(ValueTable()), },
					{ "HostValue", Value(true) },
					{ "LanguageExtensionPath", Value(std::string("C:/Extension/Language.Build.dll")), },
					{ "PackageDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/")), },
				})),
				ValueTableReader::Deserialize(testBuildGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto testBuildGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/FakeSDK/\nC:/Users/Me/.soup/packages/C#/TestBuild/1.3.0\nC:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/\n",
				testBuildGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto testBuildGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/\n",
				testBuildGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto testBuildGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/Generate.bor"));
			auto testBuildGenerateResults = OperationResultsReader::Deserialize(testBuildGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							GetMinMillisecondTime(),
							{},
							{})
					},
				}),
				testBuildGenerateResults,
				"Verify test build generate results content match expected.");

			auto myPackageGenerateParametersMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateParameters.bvt"));
			Assert::AreEqual(
				ValueTable(std::map<std::string, Value>({
					{ "ArgumentValue", Value(true) },
					{ "Dependencies", Value(ValueTable(std::map<std::string, Value>({
						{ "Build", Value(ValueTable(std::map<std::string, Value>({
							{ "TestBuild", Value(ValueTable(std::map<std::string, Value>({
								{ "Reference", Value(std::string("TestBuild@1.2.3")), },
								{ "SoupTargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/.soup/")), },
								{ "TargetDirectory", Value(std::string("C:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/")), },
							}))), },
						}))), },
					}))), },
					{ "LanguageExtensionPath", Value(std::string("C:/Extension/Language.Build.dll")), },
					{ "PackageDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/")), },
					{ "SDKs", Value(ValueList()), },
					{ "SoupTargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/")), },
					{ "TargetDirectory", Value(std::string("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/")), },
				})),
				ValueTableReader::Deserialize(myPackageGenerateParametersMockFile->Content),
				"Verify file content match expected.");

			auto myPackageGenerateReadAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateReadAccess.txt"));
			Assert::AreEqual(
				"C:/FakeSDK/\nC:/Users/Me/.soup/packages/C#/TestBuild/1.3.0/out/zDqRc65c9x3jySpevCCCyZ15fGs/\nC:/WorkingDirectory/MyPackage/\nC:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				myPackageGenerateReadAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateWriteAccessMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/GenerateWriteAccess.txt"));
			Assert::AreEqual(
				"C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/\n",
				myPackageGenerateWriteAccessMockFile->Content.str(),
				"Verify file content match expected.");

			auto myPackageGenerateResultsMockFile = fileSystem->GetMockFile(
				Path("C:/WorkingDirectory/MyPackage/out/zxAcy-Et010fdZUKLgFemwwWuC8/.soup/Generate.bor"));
			auto myPackageGenerateResults = OperationResultsReader::Deserialize(myPackageGenerateResultsMockFile->Content, fileSystemState);

			Assert::AreEqual(
				OperationResults({
					{
						1,
						OperationResult(
							true,
							GetMinMillisecondTime(),
							{},
							{})
					},
				}),
				myPackageGenerateResults,
				"Verify my package generate results content match expected.");
		}

	private:
		static std::chrono::time_point<std::chrono::system_clock> GetMinMillisecondTime()
		{
			return std::chrono::time_point_cast<std::chrono::milliseconds>(
				std::chrono::time_point<std::chrono::system_clock>::min());
		}
	};
}
