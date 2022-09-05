// <copyright file="PackageManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class PackageManagerTests
	{
	public:
		// [[Fact]]
		void RestorePackages()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto workingDirectory = Path("C:/TestLocation");
			PackageManager::RestorePackages(workingDirectory);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: RestorePackages",
					"INFO: Running PackageManager: restore-packages C:/TestLocation",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"CreateProcess: 1 [C:/testlocation/PackageManager/] C:/testlocation/PackageManager/Soup.Build.PackageManager.exe restore-packages C:/TestLocation",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				processManager->GetRequests(),
				"Verify process requests match expected.");
		}

		// [[Fact]]
		void InstallPackageReference_Version()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto workingDirectory = Path("C:/TestLocation");
			auto packageName = "TheirPackage@2.2.2";
			PackageManager::InstallPackageReference(workingDirectory, packageName);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: InstallPackageReference",
					"INFO: Running PackageManager: install-package C:/TestLocation TheirPackage@2.2.2",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"CreateProcess: 1 [C:/testlocation/PackageManager/] C:/testlocation/PackageManager/Soup.Build.PackageManager.exe install-package C:/TestLocation TheirPackage@2.2.2",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				processManager->GetRequests(),
				"Verify process requests match expected.");
		}

		// [[Fact]]
		void InstallPackageReference_NoVersion()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto workingDirectory = Path("C:/TestLocation");
			auto packageName = "TheirPackage";
			PackageManager::InstallPackageReference(workingDirectory, packageName);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: InstallPackageReference",
					"INFO: Running PackageManager: install-package C:/TestLocation TheirPackage",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"CreateProcess: 1 [C:/testlocation/PackageManager/] C:/testlocation/PackageManager/Soup.Build.PackageManager.exe install-package C:/TestLocation TheirPackage",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				processManager->GetRequests(),
				"Verify process requests match expected.");
		}

		// [[Fact]]
		void PublishPackage()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

			auto workingDirectory = Path("C:/TestLocation");
			PackageManager::PublishPackage(workingDirectory);

			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: PublishPackage",
					"INFO: Running PackageManager: publish-package C:/TestLocation",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"CreateProcess: 1 [C:/testlocation/PackageManager/] C:/testlocation/PackageManager/Soup.Build.PackageManager.exe publish-package C:/TestLocation",
					"ProcessStart: 1",
					"WaitForExit: 1",
					"GetStandardOutput: 1",
					"GetStandardError: 1",
					"GetExitCode: 1",
				}),
				processManager->GetRequests(),
				"Verify process requests match expected.");
		}
	};
}
