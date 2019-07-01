// <copyright file="IFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Path.h"

namespace Soup
{
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
        /// Gets a value indicating whether the directory/file exists
        /// </summary>
        virtual bool Exists(const Path& path) = 0;

        /// <summary>
        /// Get the last write time of the file/directory
        /// TODO: This should be a better representation of datetime...
        /// </summary>
        virtual std::time_t GetLastWriteTime(const Path& path) = 0;

        /// <summary>
        /// Open the requested file as a stream to read
        /// </summary>
        virtual std::shared_ptr<std::istream> OpenRead(const Path& path) = 0;

        /// <summary>
        /// Open the requested file as a stream to write
        /// </summary>
        virtual std::shared_ptr<std::ostream> OpenWrite(const Path& path) = 0;

    private:
        static std::shared_ptr<IFileSystem> _current;
    };

    std::shared_ptr<IFileSystem> IFileSystem::_current = nullptr;
}
