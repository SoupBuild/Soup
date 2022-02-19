// <copyright file="FileSystemStateTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class FileSystemStateTests
	{
	public:
		// [[Fact]]
		void Initialize_Default()
		{
			auto uut = FileSystemState();

			Assert::AreEqual(
				0u,
				uut.GetMaxFileId(),
				"Verify max file id match expected.");
			Assert::AreEqual(
				std::unordered_map<FileId, Path>(),
				uut.GetFiles(),
				"Verify files match expected.");
		}

		// [[Fact]]
		void Initialize_ListOperations_Single()
		{
			auto uut = FileSystemState(
				5,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}
				}));

			Assert::AreEqual(
				5u,
				uut.GetMaxFileId(),
				"Verify max file id match expected.");
			Assert::AreEqual(
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}
				}),
				uut.GetFiles(),
				"Verify files match expected.");
		}

		// [[Fact]]
		void GetFilePath_MissingThrows()
		{
			auto uut = FileSystemState(
				10,
				std::unordered_map<FileId, Path>({}));

			Assert::ThrowsRuntimeError([&uut]() {
				auto actual = uut.GetFilePath(8);
			},
			"The provided file id does not exist in the files set.");
		}

		// [[Fact]]
		void GetFilePath_Found()
		{
			auto uut = FileSystemState(
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}}));

			auto actual = uut.GetFilePath(8);
			Assert::AreEqual(Path("C:/Root/DoStuff.exe"), actual, "Verify path matches expected.");
		}

		// [[Fact]]
		void GetLastWriteTime_Missing()
		{
			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto uut = FileSystemState(
				10,
				std::unordered_map<FileId, Path>({
					{ 2, Path("C:/Root/DoStuff.exe") },
				}),
				std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::system_clock>>>({}));

			auto lastWriteTime = uut.GetLastWriteTime(2);

			Assert::AreEqual(
				std::optional<std::chrono::time_point<std::chrono::system_clock>>(std::nullopt),
				lastWriteTime,
				"Verify last write time matches expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Root/DoStuff.exe",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		// [[Fact]]
		void GetLastWriteTime_Found()
		{
			auto setLastWriteTime = std::chrono::sys_days(May/22/2015) + 9h + 11min;
			auto uut = FileSystemState(
				10,
				std::unordered_map<FileId, Path>({
					{ 2, Path("C:/Root/DoStuff.exe") },
				}),
				std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::system_clock>>>({
					{ 2, setLastWriteTime },
				}));

			auto lastWriteTime = uut.GetLastWriteTime(2);

			Assert::AreEqual(
				std::optional<std::chrono::time_point<std::chrono::system_clock>>(setLastWriteTime),
				lastWriteTime,
				"Verify last write time matches expected.");
		}

		// [[Fact]]
		void TryFindFileId_Missing()
		{
			auto uut = FileSystemState(
				10,
				std::unordered_map<FileId, Path>({}));

			FileId fileId;
			auto result = uut.TryFindFileId(Path("C:/Root/DoStuff.exe"), fileId);

			Assert::IsFalse(result, "Verify result is false.");
		}

		// [[Fact]]
		void TryFindFileId_Found()
		{
			auto uut = FileSystemState(
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}}));

			FileId fileId;
			auto result = uut.TryFindFileId(Path("C:/Root/DoStuff.exe"), fileId);

			Assert::IsTrue(result, "Verify result is true.");
			Assert::AreEqual<FileId>(8, fileId, "Verify file id matches expected.");
		}

		// [[Fact]]
		void ToFileId_Existing()
		{
			auto uut = FileSystemState(
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}}));

			FileId fileId = uut.ToFileId(Path("DoStuff.exe"), Path("C:/Root/"));

			Assert::AreEqual<FileId>(8, fileId, "Verify file id matches expected.");

			Assert::AreEqual(
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					},
				}),
				uut.GetFiles(),
				"Verify files match expected.");
		}

		// [[Fact]]
		void ToFileId_Unknown()
		{
			auto uut = FileSystemState(
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}}));

			FileId fileId = uut.ToFileId(Path("DoStuff2.exe"), Path("C:/Root/"));

			Assert::AreEqual<FileId>(11, fileId, "Verify file id matches expected.");

			Assert::AreEqual<FileId>(11, uut.GetMaxFileId(), "Verify file id matches expected.");
			Assert::AreEqual(
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					},
					{
						11,
						Path("C:/Root/DoStuff2.exe"),
					},
				}),
				uut.GetFiles(),
				"Verify files match expected.");
		}
	};
}
