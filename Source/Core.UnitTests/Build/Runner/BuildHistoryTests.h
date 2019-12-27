// <copyright file="BuildHistoryTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class BuildHistoryTests
	{
	public:
		[[Fact]]
		void TryBuildIncludeClosure_SourceFileMissingFails()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto uut = BuildHistory();

			auto sourceFiles = std::vector<Path>({
				Path("TestFile.cpp"),
			});
			auto actualClosure = std::vector<Path>();
			auto result = uut.TryBuildIncludeClosure(
				sourceFiles,
				actualClosure);

			Assert::IsFalse(result, "Verify result is false.");

			auto expectedClosure = std::vector<Path>();
			Assert::AreEqual(expectedClosure, actualClosure, "Verify the closure matches.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Missing file info: TestFile.cpp",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void TryBuildIncludeClosure_DependencyFileMissingFails()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto uut = BuildHistory(std::vector<FileInfo>({
				FileInfo(
					Path("TestFile.cpp"),
					std::vector<Path>({
						Path("OtherTestFile.h"),
					})),
			}));

			auto sourceFiles = std::vector<Path>({
				Path("TestFile.cpp"),
			});
			auto actualClosure = std::vector<Path>();
			auto result = uut.TryBuildIncludeClosure(
				sourceFiles,
				actualClosure);

			Assert::IsFalse(result, "Verify result is false.");

			auto expectedClosure = std::vector<Path>();
			Assert::AreEqual(expectedClosure, actualClosure, "Verify the closure matches.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Missing file info: OtherTestFile.h",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void TryBuildIncludeClosure_NoDependencies()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto uut = BuildHistory(std::vector<FileInfo>({
				FileInfo(
					Path("TestFile.cpp"),
					std::vector<Path>({
					})),
			}));

			auto sourceFiles = std::vector<Path>({
				Path("TestFile.cpp"),
			});
			auto actualClosure = std::vector<Path>();
			auto result = uut.TryBuildIncludeClosure(
				sourceFiles,
				actualClosure);

			Assert::IsTrue(result, "Verify result is true.");

			auto expectedClosure = std::vector<Path>({});
			Assert::AreEqual(expectedClosure, actualClosure, "Verify the closure matches.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void TryBuildIncludeClosure_MultipleDependencies()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto uut = BuildHistory(std::vector<FileInfo>({
				FileInfo(
					Path("TestFile.cpp"),
					std::vector<Path>({
						Path("TestFile1.h"),
						Path("TestFile2.h"),
					})),
				FileInfo(
					Path("TestFile1.h"),
					std::vector<Path>({
						Path("TestFile3.h"),
					})),
				FileInfo(
					Path("TestFile2.h"),
					std::vector<Path>({})),
				FileInfo(
					Path("TestFile3.h"),
					std::vector<Path>({})),
				FileInfo(
					Path("TestFile4.h"),
					std::vector<Path>({})),
			}));

			auto sourceFiles = std::vector<Path>({
				Path("TestFile.cpp"),
			});
			auto actualClosure = std::vector<Path>();
			auto result = uut.TryBuildIncludeClosure(
				sourceFiles,
				actualClosure);

			Assert::IsTrue(result, "Verify result is true.");

			auto expectedClosure = std::vector<Path>({
				Path("TestFile1.h"),
				Path("TestFile2.h"),
				Path("TestFile3.h"),
			});
			Assert::AreEqual(expectedClosure, actualClosure, "Verify the closure matches.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void TryBuildIncludeClosure_CircularDependencies()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto uut = BuildHistory(std::vector<FileInfo>({
				FileInfo(
					Path("TestFile.cpp"),
					std::vector<Path>({
						Path("TestFile1.h"),
					})),
				FileInfo(
					Path("TestFile1.h"),
					std::vector<Path>({
						Path("TestFile2.h"),
					})),
				FileInfo(
					Path("TestFile2.h"),
					std::vector<Path>({
						Path("TestFile1.h"),
					})),
			}));

			auto sourceFiles = std::vector<Path>({
				Path("TestFile.cpp"),
			});
			auto actualClosure = std::vector<Path>();
			auto result = uut.TryBuildIncludeClosure(
				sourceFiles,
				actualClosure);

			Assert::IsTrue(result, "Verify result is true.");

			auto expectedClosure = std::vector<Path>({
				Path("TestFile1.h"),
				Path("TestFile2.h"),
			});
			Assert::AreEqual(expectedClosure, actualClosure, "Verify the closure matches.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}
	};
}
