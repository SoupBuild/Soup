// <copyright file="LocalUserConfigExtensionsTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class LocalUserConfigExtensionsTests
	{
	public:
		[[Fact]]
		void GetLocalUserConfigFilePath()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>(Path("C:/TestInstall/TestProcess.exe"));
			IProcessManager::Register(processManager);

			auto result = LocalUserConfigExtensions::GetLocalUserConfigFilePath();

			Assert::AreEqual(result, Path("C:/TestInstall/LocalUserConfig.json"), "Verify the result matches expected.");
		}

		[[Fact]]
		void LoadFromFile_MissingFileThrows()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>(Path("C:/TestInstall/TestProcess.exe"));
			IProcessManager::Register(processManager);

			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			LocalUserConfig actual;
			Assert::ThrowsRuntimeError([&actual]() {
				actual = LocalUserConfigExtensions::LoadFromFile();
			});

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestInstall/LocalUserConfig.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: LocalConfig: C:/TestInstall/LocalUserConfig.json",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}
		
		[[Fact]]
		void LoadFromFile_GarbageFileThrows()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>(Path("C:/TestInstall/TestProcess.exe"));
			IProcessManager::Register(processManager);

			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);
			fileSystem->CreateMockFile(
				Path("C:/TestInstall/LocalUserConfig.json"),
				MockFileState(std::stringstream("garbage")));

			LocalUserConfig actual;
			Assert::ThrowsRuntimeError([&actual]() {
				actual = LocalUserConfigExtensions::LoadFromFile();
			});

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestInstall/LocalUserConfig.json",
					"OpenRead: C:/TestInstall/LocalUserConfig.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: LocalConfig: C:/TestInstall/LocalUserConfig.json",
					"DIAG: Deserialze Threw: Failed to parse the local user config json: expected value, got 'g' (103)",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}

		[[Fact]]
		void LoadFromFile_SimpleFile()
		{
			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>(Path("C:/TestInstall/TestProcess.exe"));
			IProcessManager::Register(processManager);

			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);
			fileSystem->CreateMockFile(
				Path("C:/TestInstall/LocalUserConfig.json"),
				MockFileState(std::stringstream(R"({
					"runtimeCompiler": "clang",
					"msvc": "../msvc/",
					"clang": "../clang/",
					"windowsSDKIncludes": [
						"windowsSDK/Include/"
					],
					"windowsSDKLibraries": [
						"windowsSDK/Library/"
					]
				})")));

			LocalUserConfig actual = LocalUserConfigExtensions::LoadFromFile();

			auto expected = LocalUserConfig(
				"clang",
				"../msvc/",
				"../clang/",
				std::vector<std::string>({
					"windowsSDK/Include/",
				}),
				std::vector<std::string>({
					"windowsSDK/Library/",
				}));

			Assert::AreEqual(expected.GetRuntimeCompiler(), actual.GetRuntimeCompiler(), "Verify matches expected.");
			Assert::AreEqual(expected.GetMSVCRootPath(), actual.GetMSVCRootPath(), "Verify matches expected.");
			Assert::AreEqual(expected.GetClangToolPath(), actual.GetClangToolPath(), "Verify matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/TestInstall/LocalUserConfig.json",
					"OpenRead: C:/TestInstall/LocalUserConfig.json",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: LocalConfig: C:/TestInstall/LocalUserConfig.json",
				}), 
				testListener->GetMessages(),
				"Verify messages match expected.");
		}
	};
}
