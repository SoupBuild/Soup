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
        /// Get the last edit time of the file/directory
        /// </summary>
        virtual std::filesystem::file_time_type GetLastEditTime(const Path& path) override final
        {
            return std::filesystem::last_write_time(path.ToString());
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
    };
}
