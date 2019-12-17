// <copyright file="STLFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"

namespace Soup
{
	/// <summary>
	/// The standard library file system
	/// </summary>
	export class STLFileSystem : public IFileSystem
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='STLFileSystem'/> class.
		/// </summary>
		STLFileSystem()
		{
		}

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		virtual bool Exists(const Path& path) override final
		{
			return std::filesystem::exists(path.ToString());
		}

		/// <summary>
		/// Get the last write time of the file/directory
		/// </summary>
		virtual std::time_t GetLastWriteTime(const Path& path) override final
		{
			#if defined ( _WIN32 )
				struct _stat64 fileInfo;
				if (_stat64(path.ToString().c_str(), &fileInfo) != 0)
					throw std::runtime_error("Failed to get last write time.");
				return fileInfo.st_mtime;
			#else
				auto fileTime = std::filesystem::last_write_time(path.ToString());
				auto time = decltype(fileTime)::clock::to_time_t(fileTime);
				return time;
			#endif
		}

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		virtual std::shared_ptr<std::istream> OpenRead(const Path& path) override final
		{
			auto file = std::make_shared<std::fstream>(path.ToString(), std::fstream::in);
			if (file->fail())
			{
				auto message = "OpenRead Failed: File missing. " + path.ToString();
				throw std::runtime_error(std::move(message));
			}

			return file;
		}

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		virtual std::shared_ptr<std::ostream> OpenWrite(const Path& path) override final
		{
			auto file = std::make_shared<std::fstream>(path.ToString(), std::fstream::out);
			return file;
		}
		
		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		virtual void CopyFile(const Path& source, const Path& destination) override final
		{
			std::filesystem::copy(
				source.ToString(),
				destination.ToString(),
				std::filesystem::copy_options::overwrite_existing);
		}

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		virtual void CreateDirectory(const Path& path) override final
		{
			std::filesystem::create_directories(path.ToString());
		}
	};
}
