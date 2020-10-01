// <copyright file="BuildHistoryChecker.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "FileSystemState.h"

namespace Soup::Build::Evaluate
{
	export class BuildHistoryChecker
	{
	public:
		BuildHistoryChecker(const FileSystemState& fileSystemState) :
			_fileSystemState(fileSystemState)
		{
		}

		/// <summary>
		/// Perform a check if the requested target is outdated with
		/// respect to the input files
		/// </summary>
		bool IsOutdated(
			const std::vector<FileId>& targetFiles,
			const std::vector<FileId>& inputFiles)
		{
			// If there are no input files then the output can never be outdated
			if (inputFiles.empty())
				return false;

			for (auto& targetFile : targetFiles)
			{
				if (IsOutdated(targetFile, inputFiles))
				{
					return true;
				}
			}

			return false;
		}

	private:
		/// <summary>
		/// Perform a check if the requested target is outdated with
		/// respect to the input files
		/// </summary>
		bool IsOutdated(
			FileId targetFile,
			const std::vector<FileId>& inputFiles)
		{
			// Get the output file last write time
			std::optional<time_t> targetFileLastWriteTime;
			if (_fileSystemState.TryGetLastWriteTime(targetFile, targetFileLastWriteTime))
			{
				if (!targetFileLastWriteTime.has_value())
				{
					auto targetFilePath = _fileSystemState.GetFilePath(targetFile);
					Log::Info("Output target does not exist: " + targetFilePath.ToString());
					return true;
				}

				// Note: No need to use cache here since target files should only be analyzed once
				for (auto& inputFile : inputFiles)
				{
					// If the file is relative then combine it with the root path
					if (IsOutdated(inputFile, targetFile, targetFileLastWriteTime.value()))
					{
						return true;
					}
				}

				return false;
			}
			else
			{
				Log::Warning("Output file missing from file system state.");
				return true;
			}
		}

		bool IsOutdated(FileId inputFile, FileId outputFile, std::time_t outputFileLastWriteTime)
		{
			// Get the file state from the cache
			std::optional<time_t> lastWriteTime;
			if (_fileSystemState.TryGetLastWriteTime(inputFile, lastWriteTime))
			{
				// Perform the final check
				if (!lastWriteTime.has_value())
				{
					// The input was missing
					auto inputFilePath = _fileSystemState.GetFilePath(inputFile);
					Log::Info("Input Missing [" + inputFilePath.ToString() + "]");
					return true;
				}
				else
				{
					if (lastWriteTime.value() > outputFileLastWriteTime)
					{
						auto inputFilePath = _fileSystemState.GetFilePath(inputFile);
						auto outputFilePath = _fileSystemState.GetFilePath(outputFile);
						Log::Info("Input altered after target [" + inputFilePath.ToString() + "] -> [" + outputFilePath.ToString() + "]");
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			else
			{
				Log::Warning("Input file missing from file system state.");
				return true;
			}
		}

	private:
		const FileSystemState& _fileSystemState;
	};
}
