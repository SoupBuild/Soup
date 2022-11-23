// <copyright file="FileSystemState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	export using FileId = uint32_t;

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
			_maxFileId(0),
			_files(),
			_fileLookup(),
			_writeCache()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="FileSystemState"/> class.
		/// </summary>
		FileSystemState(
			FileId maxFileId,
			std::unordered_map<FileId, Path> files) :
			FileSystemState(maxFileId, std::move(files), {})
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="FileSystemState"/> class.
		/// </summary>
		FileSystemState(
			FileId maxFileId,
			std::unordered_map<FileId, Path> files,
			std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::file_clock>>> writeCache) :
			_maxFileId(maxFileId),
			_files(std::move(files)),
			_fileLookup(),
			_writeCache(std::move(writeCache))
		{
			// Build up the reverse lookup for new files
			for (auto& file : _files)
			{
				auto normalizedFilePath = file.second.ToString();
				ToUpper(normalizedFilePath);
				auto insertResult = _fileLookup.emplace(std::move(normalizedFilePath), file.first);
				if (!insertResult.second)
					throw std::runtime_error("The file was not unique in the provided set.");
			}
		}

		/// <summary>
		/// Get Files
		/// </summary>
		const std::unordered_map<FileId, Path>& GetFiles() const
		{
			return _files;
		}

		/// <summary>
		/// Get the max unique file id
		/// </summary>
		FileId GetMaxFileId() const
		{
			return _maxFileId;
		}

		/// <summary>
		/// Update the write times for the provided set of files
		/// </summary>
		void InvalidateFileWriteTimes(const std::vector<FileId>& files)
		{
			for (auto file : files)
			{
				InvalidateFileWriteTime(file);
			}
		}

		/// <summary>
		/// Find the write time for a given file id
		/// </summary>
		std::optional<std::chrono::time_point<std::chrono::file_clock>> GetLastWriteTime(FileId file)
		{
			auto findResult = _writeCache.find(file);
			if (findResult != _writeCache.end())
			{
				return findResult->second;
			}
			else
			{
				return CheckFileWriteTime(file);
			}
		}

		/// <summary>
		/// Convert a set of file paths to file ids
		/// </summary>
		std::vector<FileId> ToFileIds(const std::vector<Path>& files, const Path& workingDirectory)
		{
			auto result = std::vector<FileId>();
			for (auto& file : files)
			{
				result.push_back(ToFileId(file, workingDirectory));
			}

			return result;
		}

		/// <summary>
		/// Convert a file path to file id
		/// </summary>
		FileId ToFileId(const Path& file, const Path& workingDirectory)
		{
			auto& absolutePath = file.HasRoot() ? file : workingDirectory + file;
			return ToFileId(absolutePath);
		}

		FileId ToFileId(const Path& file)
		{
			if (!file.HasRoot())
				throw std::runtime_error("File paths must be absolute to resolve to an id");

			// Check if the file is already known
			FileId result;
			if (!TryFindFileId(file, result))
			{
				// Insert the new file
				result = ++_maxFileId;
				auto insertResult = _files.emplace(result, file);
				if (!insertResult.second)
					throw std::runtime_error("The provided file id already exists in the file system state");

				auto normalizedFilePath = file.ToString();
				ToUpper(normalizedFilePath);
				auto insertLookupResult = _fileLookup.emplace(std::move(normalizedFilePath), result);
				if (!insertLookupResult.second)
					throw std::runtime_error("The file was not unique even though we just failed to find it");
			}

			return result;
		}

		/// <summary>
		/// Find an file id
		/// </summary>
		bool TryFindFileId(const Path& file, FileId& fileId) const
		{
			auto normalizedFilePath = file.ToString();
			ToUpper(normalizedFilePath);
			auto findResult = _fileLookup.find(normalizedFilePath);
			if (findResult != _fileLookup.end())
			{
				fileId = findResult->second;
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Find a file path
		/// </summary>
		std::vector<Path> GetFilePaths(const std::vector<FileId>& fileIds) const
		{
			auto result = std::vector<Path>();
			for (auto& fileId : fileIds)
			{
				result.push_back(GetFilePath(fileId));
			}

			return result;
		}

		/// <summary>
		/// Find a file path
		/// </summary>
		const Path& GetFilePath(FileId fileId) const
		{
			auto findResult = _files.find(fileId);
			if (findResult != _files.end())
			{
				return findResult->second;
			}
			else
			{
				throw std::runtime_error("The provided file id does not exist in the files set.");
			}
		}

	private:
		/// <summary>
		/// Invalidate the write time for the provided file
		/// </summary>
		void InvalidateFileWriteTime(FileId fileId)
		{
			_writeCache.erase(fileId);
		}

		/// <summary>
		/// Update the write times for the provided file
		/// </summary>
		std::optional<std::chrono::time_point<std::chrono::file_clock>> CheckFileWriteTime(FileId fileId)
		{
			#ifdef USE_SINGLE_FILE_CHECK
				auto& filePath = GetFilePath(fileId);

				// The file does not exist in the cache
				// Load the actual value and save it for later
				std::optional<std::chrono::time_point<std::chrono::file_clock>> lastWriteTime = std::nullopt;
				std::chrono::time_point<std::chrono::file_clock> lastWriteTimeValue;
				if (System::IFileSystem::Current().TryGetLastWriteTime(filePath, lastWriteTimeValue))
				{
					lastWriteTime = lastWriteTimeValue;
				}

				auto insertResult = _writeCache.insert_or_assign(fileId, lastWriteTime);
				return lastWriteTime;
			#else
				auto& filePath = GetFilePath(fileId);
				
				// Add the requested file as null
				// This will be replaced if the file exists with the find all callback
				auto insertResult = _writeCache.insert_or_assign(fileId, std::nullopt);

				// Load the write times for all files in the directory
				// This optimization assumes that most files in a directory are relevant to the build
				// and on windows it is a lot faster to iterate over the files instead of making individual calls
				auto parentDirectory = filePath.GetParent();
				System::IFileSystem::Current().GetDirectoryFilesLastWriteTime(
					parentDirectory,
					[&](const Path& file, std::chrono::time_point<std::chrono::file_clock> lastWriteTime)
					{
						auto& absolutePath = file.HasRoot() ? file : parentDirectory + file;
						FileId result;
						if (TryFindFileId(absolutePath, result))
						{
							auto insertResult = _writeCache.insert_or_assign(result, lastWriteTime);
						}
					});

				return _writeCache[fileId];
			#endif
		}

		static void ToUpper(std::string& value)
		{
			std::transform(
				value.begin(),
				value.end(),
				value.begin(),
				[](unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); });
		}

	private:
		// The maximum id that has been used for files
		// Used to ensure unique ids are generated across the entire system
		FileId _maxFileId;

		std::unordered_map<FileId, Path> _files;
		std::unordered_map<std::string, FileId> _fileLookup;

		std::unordered_map<FileId, std::optional<std::chrono::time_point<std::chrono::file_clock>>> _writeCache;
	};
}
