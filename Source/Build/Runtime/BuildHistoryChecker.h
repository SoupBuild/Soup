// <copyright file="BuildHistoryChecker.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "FileSystemState.h"

namespace Soup::Build::Runtime
{
	export class BuildHistoryChecker
	{
	public:
		BuildHistoryChecker(FileSystemState& fileSystemState) :
			_fileSystemState(fileSystemState)
		{
		}

		/// <summary>
		/// Perform a check if the requested target is outdated with
		/// respect to the input files
		/// </summary>
		bool IsOutdated(
			const Path& workingDirectory,
			const std::vector<Path>& targetFiles,
			const std::vector<Path>& inputFiles)
		{
			// If there are no input files then the output can never be outdated
			if (inputFiles.empty())
				return false;

			// Ensure all of the target files are absolute paths
			auto rootedInputFiles = std::vector<Path>();
			for (auto& inputFile : inputFiles)
			{
				auto rootedInputFile = inputFile.HasRoot() ? inputFile : workingDirectory + inputFile;
				rootedInputFiles.push_back(std::move(rootedInputFile));
			}

			for (auto& targetFile : targetFiles)
			{
				auto rootedTargetFile = targetFile.HasRoot() ? targetFile : workingDirectory + targetFile;
				if (IsOutdated(rootedTargetFile, rootedInputFiles))
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
			const Path& targetFile,
			const std::vector<Path>& inputFiles)
		{
			// Get the output file last write time
			std::optional<time_t> targetFileLastWriteTime = _fileSystemState.GetLastWriteTime(targetFile);

			if (!targetFileLastWriteTime.has_value())
			{
				Log::Info("Output target does not exist: " + targetFile.ToString());
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

		bool IsOutdated(const Path& inputFile, const Path& outputFile, std::time_t outputFileLastWriteTime)
		{
			// Get the file state from the cache
			std::optional<time_t> lastWriteTime = _fileSystemState.GetLastWriteTime(inputFile);

			// Perform the final check
			if (!lastWriteTime.has_value())
			{
				// The input was missing
				Log::Info("Input Missing [" + inputFile.ToString() + "]");
				return true;
			}
			else
			{
				if (lastWriteTime.value() > outputFileLastWriteTime)
				{
					Log::Info("Input altered after target [" + inputFile.ToString() + "] -> [" + outputFile.ToString() + "]");
					return true;
				}
				else
				{
					return false;
				}
			}
		}

	private:
		FileSystemState& _fileSystemState;
	};
}
