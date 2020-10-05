// <copyright file="FileSystemStateTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime::UnitTests
{
	class FileSystemStateTests
	{
	public:
		[[Fact]]
		void Initialize_Default()
		{
			auto uut = FileSystemState(1234);

			Assert::AreEqual(
				1234u,
				uut.GetId(),
				"Verify id match expected.");
			Assert::AreEqual(
				0u,
				uut.GetMaxFileId(),
				"Verify max file id match expected.");
			Assert::AreEqual(
				std::unordered_map<FileId, Path>(),
				uut.GetFiles(),
				"Verify files match expected.");
		}

		[[Fact]]
		void Initialize_ListOperations_Single()
		{
			auto uut = FileSystemState(
				1234,
				5,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}
				}));

			Assert::AreEqual(
				1234u,
				uut.GetId(),
				"Verify id match expected.");
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

		[[Fact]]
		void LoadCurrentFileSystemState_Empty()
		{
			auto uut = FileSystemState(
				1234,
				5,
				std::unordered_map<FileId, Path>({}));

			uut.LoadCurrentFileSystemState();
		}

		[[Fact]]
		void LoadCurrentFileSystemState_SingleMissing()
		{
			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto uut = FileSystemState(
				1234,
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}
				}));

			uut.LoadCurrentFileSystemState();

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Root/DoStuff.exe",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void LoadCurrentFileSystemState_SingleExists()
		{
			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

			auto lastWriteTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(
				Path("C:/Root/DoStuff.exe"),
				std::make_shared<MockFile>(lastWriteTime));

			auto uut = FileSystemState(
				1234,
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}
				}));

			uut.LoadCurrentFileSystemState();

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::string>({
					"Exists: C:/Root/DoStuff.exe",
					"GetLastWriteTime: C:/Root/DoStuff.exe",
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void GetFilePath_MissingThrows()
		{
			auto uut = FileSystemState(
				1234,
				10,
				std::unordered_map<FileId, Path>({}));

			Assert::ThrowsRuntimeError([&uut]() {
				auto actual = uut.GetFilePath(8);
			},
			"The provided file id does not exist in the files set.");
		}

		[[Fact]]
		void GetFilePath_Found()
		{
			auto uut = FileSystemState(
				1234,
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}}));

			auto actual = uut.GetFilePath(8);
			Assert::AreEqual(Path("C:/Root/DoStuff.exe"), actual, "Verify path matches expected.");
		}

		[[Fact]]
		void TryGetLastWriteTime_Missing()
		{
			auto uut = FileSystemState(
				1234,
				10,
				std::unordered_map<FileId, Path>({}));

			std::optional<time_t> lastWriteTime;
			auto result = uut.TryGetLastWriteTime(8, lastWriteTime);

			Assert::IsFalse(result, "Verify result is false.");
		}

		[[Fact]]
		void TryGetLastWriteTime_Found()
		{
			auto uut = FileSystemState(
				1234,
				10,
				std::unordered_map<FileId, Path>({
					{
						8,
						Path("C:/Root/DoStuff.exe"),
					}}));

			auto setLastWriteTime = CreateDateTime(2015, 5, 22, 9, 11);
			uut.SetLastWriteTime(8, setLastWriteTime);


			std::optional<time_t> lastWriteTime;
			auto result = uut.TryGetLastWriteTime(8, lastWriteTime);

			Assert::IsTrue(result, "Verify result is true.");
			Assert::AreEqual(std::optional<time_t>(setLastWriteTime), lastWriteTime, "Verify last write time matches expected.");
		}

		[[Fact]]
		void TryFindFileId_Missing()
		{
			auto uut = FileSystemState(
				1234,
				10,
				std::unordered_map<FileId, Path>({}));

			FileId fileId;
			auto result = uut.TryFindFileId(Path("C:/Root/DoStuff.exe"), fileId);

			Assert::IsFalse(result, "Verify result is false.");
		}

		[[Fact]]
		void TryFindFileId_Found()
		{
			auto uut = FileSystemState(
				1234,
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

		[[Fact]]
		void ToFileId_Existing()
		{
			auto uut = FileSystemState(
				1234,
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

		[[Fact]]
		void ToFileId_Unknown()
		{
			auto uut = FileSystemState(
				1234,
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
