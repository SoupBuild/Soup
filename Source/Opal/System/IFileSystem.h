// <copyright file="IFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IInputFile.h"
#include "IOutputFile.h"

namespace Opal::System
{
	export struct DirectoryEntry
	{
		Path Path;
		bool IsDirectory;
	};

	/// <summary>
	/// The file system interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class IFileSystem
	{
	public:
		/// <summary>
		/// Gets the current active file system
		/// </summary>
		static IFileSystem& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No file system implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active file system
		/// </summary>
		static void Register(std::shared_ptr<IFileSystem> fileSystem)
		{
			_current = std::move(fileSystem);
		}

	public:
		/// <summary>
		/// Gets the current user profile directory
		/// </summary>
		virtual Path GetUserProfileDirectory() = 0;

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		virtual Path GetCurrentDirectory2() = 0;

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		virtual bool Exists(const Path& path) = 0;

		/// <summary>
		/// Get the last write time of the file/directory
		/// TODO: This should be a better representation of datetime...
		/// </summary>
		virtual std::time_t GetLastWriteTime(const Path& path) = 0;

		/// <summary>
		/// Set the last write time of the file/directory
		/// TODO: This should be a better representation of datetime...
		/// </summary>
		virtual void SetLastWriteTime(const Path& path, std::time_t value) = 0;

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		virtual std::shared_ptr<IInputFile> OpenRead(const Path& path, bool isBinary) = 0;

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		virtual std::shared_ptr<IOutputFile> OpenWrite(const Path& path, bool isBinary) = 0;

		/// <summary>
		/// Rename the source file to the destination
		/// </summary>
		virtual void Rename(const Path& source, const Path& destination) = 0;

		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		virtual void CopyFile2(const Path& source, const Path& destination) = 0;

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		virtual void CreateDirectory2(const Path& path) = 0;

		/// <summary>
		/// Get the children of a directory
		/// </summary>
		virtual std::vector<DirectoryEntry> GetDirectoryChildren(const Path& path) = 0;

		/// <summary>
		/// Delete the directory
		/// </summary>
		virtual void DeleteDirectory(const Path& path, bool recursive) = 0;

	private:
		static std::shared_ptr<IFileSystem> _current;
	};

	std::shared_ptr<IFileSystem> IFileSystem::_current = nullptr;
}
