// <copyright file="FileSystemState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The complete set of known files that tracking the active change state during execution
	/// </summary>
	export class FileSystemState
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="FileSystemState"/> class.
		/// </summary>
		FileSystemState() :
			_writeCache()
		{
		}

		/// <summary>
		/// Update the write times for the provided set of files
		/// </summary>
		void CheckFileWriteTimes(const std::vector<Path>& files)
		{
			for (auto file : files)
			{
				// The file does not exist in the cache
				// Load the actual value and save it for later
				std::optional<std::time_t> lastWriteTime = std::nullopt;
				if (System::IFileSystem::Current().Exists(file))
				{
					lastWriteTime = System::IFileSystem::Current().GetLastWriteTime(file);
				}

				SetLastWriteTime(file, lastWriteTime);
			}
		}

		/// <summary>
		/// Find the write time for a given file id
		/// </summary>
		std::optional<time_t> GetLastWriteTime(const Path& file)
		{
			auto findResult = _writeCache.find(file.ToString());
			if (findResult != _writeCache.end())
			{
				return findResult->second;
			}
			else
			{
				return CheckFileWriteTime(file);
			}
		}

	private:
		/// <summary>
		/// Update the write times for the provided set of files
		/// </summary>
		std::optional<time_t> CheckFileWriteTime(const Path& file)
		{
			// The file does not exist in the cache
			// Load the actual value and save it for later
			std::optional<std::time_t> lastWriteTime = std::nullopt;
			if (System::IFileSystem::Current().Exists(file))
			{
				lastWriteTime = System::IFileSystem::Current().GetLastWriteTime(file);
			}

			SetLastWriteTime(file, lastWriteTime);
			return lastWriteTime;
		}

		/// <summary>
		/// Update the last write time for the provided file id
		/// </summary>
		void SetLastWriteTime(const Path& file, std::optional<time_t> value)
		{
			auto insertResult = _writeCache.insert_or_assign(file.ToString(), value);
		}

	private:
		std::unordered_map<std::string, std::optional<time_t>> _writeCache;
	};
}
