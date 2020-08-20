// <copyright file="LzmaUpdateCallback.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "LzmaInStream.h"

namespace Soup
{
	/// <summary>
	/// A callback interface implementation to get progress updates and provide integration
	/// with the filesystem during Updateion of an archive.
	/// </summary>
	class LzmaUpdateCallback : public LzmaSdk::IArchiveUpdateCallback
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LzmaUpdateCallback"/> class.
		/// </summary>
		LzmaUpdateCallback(Path workingDirectory) :
			_workingDirectory(std::move(workingDirectory))
		{
		}

		// Progress Updates
		void OnStart(uint64_t size) override final
		{
			Log::Info("UpdateStart: " + std::to_string(size));
			_size = size;
		}

		void OnProgress(uint64_t value) override final
		{
			double percent = 0;
			if (_size > 0)
				percent = value / (double)_size;

			Log::Info("UpdateProgress: " + std::to_string(percent));
		}

		void OnCompleted() override final
		{
			Log::Info("UpdateDone");
		}

		// Filesystem access requests
		std::shared_ptr<LzmaSdk::ISequentialInStream> GetStream(
			std::string_view path) override final
		{
			auto currentFile = Path(path);
			Log::Info("UpdateGetStream: " + currentFile.ToString());

			auto currentFilePath = _workingDirectory + currentFile;
			auto currentFileStream = System::IFileSystem::Current().OpenRead(currentFilePath, true);
			auto outStream = std::make_shared<LzmaInStream>(currentFileStream);

			return outStream;
		}

	private:
		Path _workingDirectory;
		uint64_t _size;
	};
}
