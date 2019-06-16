// <copyright file="MockFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"

namespace Soup
{
    export enum class FileSystemRequestType
    {
        Exists,
        GetLastEditTime,
        OpenRead,
        OpenWrite
    };

    /// <summary>
    /// The mock file system
    /// TODO: Move into test project
    /// </summary>
    export class MockFileSystem : public IFileSystem
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref='MockFileSystem'/> class.
        /// </summary>
        MockFileSystem() :
            _requests(),
            _files()
        {
        }

        /// <summary>
        /// Get the load requests
        /// </summary>
        const std::vector<std::pair<std::string, FileSystemRequestType>>& GetRequests() const
        {
            return _requests;
        }

        /// <summary>
        /// Create a test file
        /// </summary>
        void CreateFile(Path path, std::string contents)
        {
            _files.emplace(path, std::make_shared<std::stringstream>(contents));
        }

        /// <summary>
        /// Gets a value indicating whether the directory/file exists
        /// </summary>
        virtual bool Exists(const Path& path) override final
        {
            _requests.push_back(std::make_pair(path.ToString(), FileSystemRequestType::Exists));
            auto file = _files.find(path);
            return file != _files.end();
        }

        /// <summary>
        /// Get the last edit time of the file/directory
        /// </summary>
        virtual std::filesystem::file_time_type GetLastEditTime(const Path& path) override final
        {
            _requests.push_back(std::make_pair(path.ToString(), FileSystemRequestType::GetLastEditTime));
            auto message = "Cannot get last edit time: " + path.ToString();
            throw std::runtime_error(message);
        }

        /// <summary>
        /// Open the requested file as a stream to read
        /// </summary>
        virtual std::shared_ptr<std::istream> OpenRead(const Path& path) override final
        {
            _requests.push_back(std::make_pair(path.ToString(), FileSystemRequestType::OpenRead));
            auto file = _files.find(path);
            if (file != _files.end())
            {
                return file->second;
            }
            else
            {
                auto message = "Cannot open read: " + path.ToString();
                throw std::runtime_error(message);
            }
        }

        /// <summary>
        /// Open the requested file as a stream to write
        /// </summary>
        virtual std::shared_ptr<std::ostream> OpenWrite(const Path& path) override final
        {
            _requests.push_back(std::make_pair(path.ToString(), FileSystemRequestType::OpenWrite));
            auto file = _files.find(path);
            if (file != _files.end())
            {
                return file->second;
            }
            else
            {
                // Create the file if it does not exist
                auto insert = _files.emplace(path, std::make_shared<std::stringstream>());
                return insert.first->second;
            }
        }

    private:
        std::vector<std::pair<std::string, FileSystemRequestType>> _requests;
        std::map<Path, std::shared_ptr<std::stringstream>> _files;
    };
}
