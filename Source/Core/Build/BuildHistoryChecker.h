// <copyright file="BuildHistoryChecker.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildHistory.h"

namespace Soup
{
	export class BuildHistoryChecker
	{
	public:
		BuildHistoryChecker() :
			m_cache()
		{
		}

		/// <summary>
		/// Perform a check if the requested target is outdated with
		/// respect to the input files
		/// </summary>
		bool IsOutdated(
			const Path& targetFile,
			const std::vector<Path>& inputFiles,
			const Path& rootPath)
		{
			if (!rootPath.HasRoot())
				throw std::runtime_error("The root path must have a root.");
			if (inputFiles.empty())
				throw std::runtime_error("Cannot check outdated with no input files.");

			// Verify the output file exists
			auto relativeOutputFile = rootPath + targetFile;
			if (!System::IFileSystem::Current().Exists(relativeOutputFile))
			{
				Log::Info("Output target does not exist: " + relativeOutputFile.ToString());
				return true;
			}

			// Note: No need to use cache here since target files should only be analyzed once
			auto outputFileLastWriteTime = 
				System::IFileSystem::Current().GetLastWriteTime(relativeOutputFile);
			Log::Diag("IsOutdated: " + relativeOutputFile.ToString() + " [" + std::to_string(outputFileLastWriteTime) + "]");
			for (auto& inputFile : inputFiles)
			{
				// If the file is relative then combine it with the root path
				auto relativeInputFile = inputFile.HasRoot() ? inputFile : rootPath + inputFile;
				if (IsOutdated(relativeInputFile, relativeOutputFile, outputFileLastWriteTime))
				{
					return true;
				}
			}

			return false;
		}

	private:
		bool IsOutdated(Path inputFile, Path outputFile, std::time_t outputFileLastWriteTime)
		{
			// Check if the file exists in the cache
			std::optional<std::time_t> lastWriteTime = std::nullopt;
			auto search = m_cache.find(inputFile.ToString());
			if (search != m_cache.end())
			{
				lastWriteTime = search->second;
			}
			else
			{
				// The file does not exist in the cache
				// Load the actual value and save it for later
				if (System::IFileSystem::Current().Exists(inputFile))
				{
					lastWriteTime = System::IFileSystem::Current().GetLastWriteTime(inputFile);
				}
				else
				{
					lastWriteTime = std::nullopt;
				}

				// Store the result for later
				m_cache.emplace(inputFile.ToString(), lastWriteTime);
			}

			// Perform the final check
			if (!lastWriteTime.has_value())
			{
				// The input 
				Log::Error("  " + inputFile.ToString() + " [MISSING]");
				return true;
			}
			else
			{
				Log::Diag("  " + inputFile.ToString() + " [" + std::to_string(lastWriteTime.value()) + "]");
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

		std::unordered_map<std::string, std::optional<time_t>> m_cache;
	};
}
