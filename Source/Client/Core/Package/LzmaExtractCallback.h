// <copyright file="LzmaExtractCallback.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "LzmaSequentialOutStream.h"

namespace Soup
{
	/// <summary>
	/// A callback interface implementation to get progress updates and provide integration
	/// with the filesystem during extraction of an archive.
	/// </summary>
	class LzmaExtractCallback : public LzmaSdk::IArchiveExtractCallback
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LzmaExtractCallback"/> class.
		/// </summary>
		LzmaExtractCallback()
		{
		}

		// Progress Updates
		void OnStart(uint64_t size) override final
		{
			Log::Info("ExtractStart: " + std::to_string(size));
		}

		void OnProgress(uint64_t value) override final
		{
			Log::Info("ExtractProgress: " + std::to_string(value));
		}

		void OnCompleted() override final
		{
			Log::Info("ExtractDone");
		}

		// Filesystem access requests
		std::shared_ptr<LzmaSdk::ISequentialOutStream> GetStream(
			std::string_view path,
			std::optional<std::time_t> modifiedTime,
			std::optional<uint32_t> permissions) override final
		{
			_currentFilePath = Path(path);
			Log::Info("ExtractGetStream: " + _currentFilePath.ToString());

			// Save the properties to set after finished
			_currentFileModifiedTime = modifiedTime;

			// Ensure the containing directory exists
			auto parentDirectory = _currentFilePath.GetParent();
			if (!System::IFileSystem::Current().Exists(parentDirectory))
			{
				// Create the folder
				System::IFileSystem::Current().CreateDirectory2(parentDirectory);
			}

			_currentFile = System::IFileSystem::Current().OpenWrite(_currentFilePath, true);
			auto outStream = std::make_shared<LzmaSequentialOutStream>(_currentFile);

			return outStream;
		}

		// Operation progress
		void OnOperationStart(LzmaSdk::OperationType type) override final
		{
			Log::Info("ExtractOnOperationStart");
		}

		void OnOperationCompleted(LzmaSdk::OperationResult result) override final
		{
			Log::Info("ExtractOperationCompleted");

			// Close the output file
			if (_currentFile != nullptr)
			{
				_currentFile->Close();
				_currentFile = nullptr;
			}

			// Set file properties at the end
			if (_currentFileModifiedTime.has_value())
			{
				System::IFileSystem::Current().SetLastWriteTime(
					_currentFilePath,
					_currentFileModifiedTime.value());
				_currentFileModifiedTime = std::nullopt;
			}

			// if (_extractMode && _processedFileInfo.AttribDefined)
			// 	NWindows::NFile::NDir::SetFileAttrib_PosixHighDetect(_diskFilePath, _processedFileInfo.Attrib);
		}

	private:
		Path _currentFilePath;
		std::shared_ptr<System::IOutputFile> _currentFile;
		std::optional<std::time_t> _currentFileModifiedTime;
	};
}
