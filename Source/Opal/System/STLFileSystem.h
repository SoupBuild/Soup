// <copyright file="STLFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"
#include "STLInputFile.h"
#include "STLOutputFile.h"

namespace Opal::System
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
		/// Gets the current user profile directory
		/// </summary>
		Path GetUserProfileDirectory() override final
		{
			#ifdef _WIN32

			char userProfile[MAX_PATH];
			HRESULT result = SHGetFolderPath(
				nullptr,
				CSIDL_PROFILE,
				nullptr,
				SHGFP_TYPE_CURRENT,
				userProfile);
			if (result != S_OK)
				throw std::runtime_error("SHGetFolderPath failed.");

			return Path(userProfile);

			#else
			#error NOT IMPLEMENTED
			#endif
		}

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		Path GetCurrentDirectory2() override final
		{
			auto current = std::filesystem::current_path();
			return Path(current.string());
		}

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		bool Exists(const Path& path) override final
		{
			return std::filesystem::exists(path.ToString());
		}

		/// <summary>
		/// Get the last write time of the file/directory
		/// </summary>
		std::time_t GetLastWriteTime(const Path& path) override final
		{
			// TODO: Remove when C++20 is ready
			#if defined ( _WIN32 )
				struct _stat64 fileInfo;
				if (_stat64(path.ToString().c_str(), &fileInfo) != 0)
					throw std::runtime_error("Failed to get last write time.");
				return fileInfo.st_mtime;
			#else
				auto fileTime = std::filesystem::last_write_time(path.ToString());
				auto systemTime = std::chrono::file_time::to_sys(fileTime);
				auto time = std::chrono::system_time::clock::to_time_t(systemTime);
				return time;
			#endif
		}

		/// <summary>
		/// Set the last write time of the file/directory
		/// </summary>
		void SetLastWriteTime(const Path& path, std::time_t value) override final
		{
			// TODO: Remove when C++20 is ready
			#if defined ( _WIN32 )
				// Open a handle on the file
				auto fileHandle = CreateFile(
					path.ToString().c_str(),
					GENERIC_WRITE,
					FILE_SHARE_READ,
					nullptr,
					OPEN_EXISTING,
					0,
					nullptr);

				auto fileTime = TimetToFileTime(value);

				if (!SetFileTime(
					fileHandle,
					(LPFILETIME)nullptr,
					(LPFILETIME)nullptr, 
					&fileTime))
				{
					auto error = GetLastError();
					throw std::runtime_error("Failed to set file time: " + std::to_string(error));
				}

				if (!CloseHandle(fileHandle))
				{
					auto error = GetLastError();
					throw std::runtime_error("Failed to close file handle: " + std::to_string(error));
				}
			#else
				auto systemTime = std::chrono::system_clock::from_time_t(value);
				auto fileTime = std::chrono::file_time::from_sys(systemTime);
				std::filesystem::last_write_time(path.ToString(), fileTime);
			#endif
		}

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		std::shared_ptr<IInputFile> OpenRead(const Path& path, bool isBinary) override final
		{
			int mode = std::fstream::in;
			if (isBinary)
			{
				mode = mode | std::fstream::binary;
			}

			auto file = std::fstream(path.ToString(), mode);
			if (file.fail())
			{
				auto message = "OpenRead Failed: File missing. " + path.ToString();
				throw std::runtime_error(std::move(message));
			}

			return std::make_shared<STLInputFile>(std::move(file));
		}

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		std::shared_ptr<IOutputFile> OpenWrite(const Path& path, bool isBinary) override final
		{
			int mode = std::fstream::out;
			if (isBinary)
			{
				mode = mode | std::fstream::binary;
			}

			auto file = std::fstream(path.ToString(), mode);
			if (file.fail())
			{
				auto message = "OpenWrite Failed: " + path.ToString();
				throw std::runtime_error(std::move(message));
			}

			return std::make_shared<STLOutputFile>(std::move(file));
		}

		/// <summary>
		/// Rename the source file to the destination
		/// </summary>
		virtual void Rename(const Path& source, const Path& destination) override final
		{
			std::filesystem::rename(
				source.ToString(),
				destination.ToString());
		}

		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		void CopyFile2(const Path& source, const Path& destination) override final
		{
			std::filesystem::copy(
				source.ToString(),
				destination.ToString(),
				std::filesystem::copy_options::overwrite_existing);
		}

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		void CreateDirectory2(const Path& path) override final
		{
			std::filesystem::create_directories(path.ToString());
		}

		/// <summary>
		/// Get the children of a directory
		/// </summary>
		std::vector<DirectoryEntry> GetDirectoryChildren(const Path& path) override final
		{
			auto result = std::vector<DirectoryEntry>();
			for(auto& child : std::filesystem::directory_iterator(path.ToString()))
			{
				result.push_back(
					{
						Path(child.path().string()),
						child.is_directory()
					});
			}

			return result;
		}

		/// <summary>
		/// Delete the directory
		/// </summary>
		void DeleteDirectory(const Path& path, bool recursive) override final
		{
			if (recursive)
			{
				std::filesystem::remove_all(path.ToString());
			}
			else
			{
				std::filesystem::remove(path.ToString());
			}
		}

	private:
		FILETIME TimetToFileTime(std::time_t time)
		{
			LONGLONG ll = Int32x32To64(time, 10000000) + 116444736000000000;
			FILETIME result;
			result.dwLowDateTime = (DWORD) ll;
			result.dwHighDateTime = ll >>32;

			return result;
		}
	};
}
