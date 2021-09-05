// <copyright file="RuntimeFileSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.IO;

namespace Opal.System
{
    /// <summary>
    /// The runtime library file system
    /// </summary>
    public class RuntimeFileSystem : IFileSystem
    {
        /// <summary>
        /// Initializes a new instance of the <see cref='RuntimeFileSystem'/> class.
        /// </summary>
        public RuntimeFileSystem()
        {
        }

        /// <summary>
        /// Gets the current user profile directory
        /// </summary>
        public Path GetUserProfileDirectory()
        {
            var userProfileFolder = Environment.GetEnvironmentVariable("USERPROFILE");
            if (userProfileFolder == null)
                throw new InvalidOperationException("Unable to retrieve user profile");
            return new Path(userProfileFolder);
        }

        /// <summary>
        /// Gets the current directory for the running processes
        /// </summary>
        public Path GetCurrentDirectory2()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Gets a value indicating whether the directory/file exists
        /// </summary>
        public bool Exists(Path path)
        {
            return File.Exists(path.ToString());
        }

        /// <summary>
        /// Get the last write time of the file/directory
        /// </summary>
        public int GetLastWriteTime(Path path)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Set the last write time of the file/directory
        /// </summary>
        public void SetLastWriteTime(Path path, int value)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Open the requested file as a stream to read
        /// </summary>
        public IInputFile OpenRead(Path path, bool isBinary)
        {
            return new RuntimeInputFile(File.OpenRead(path.ToString()));
        }

        /// <summary>
        /// Open the requested file as a stream to write
        /// </summary>
        public IOutputFile OpenWrite(Path path, bool isBinary)
        {
            return new RuntimeOutputFile(File.OpenWrite(path.ToString()));
        }

        /// <summary>
        /// Rename the source file to the destination
        /// </summary>
        public virtual void Rename(Path source, Path destination)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Copy the source file to the destination
        /// </summary>
        public void CopyFile2(Path source, Path destination)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Create the directory at the requested path
        /// </summary>
        public void CreateDirectory2(Path path)
        {
            Directory.CreateDirectory(path.ToString());
        }

        /// <summary>
        /// Get the children of a directory
        /// </summary>
        public IReadOnlyList<DirectoryEntry> GetDirectoryChildren(Path path)
        {
            var result = new List<DirectoryEntry>();
            foreach (var directory in Directory.EnumerateDirectories(path.ToString()))
            {
                result.Add(new DirectoryEntry()
                {
                    Path = new Path(directory),
                    IsDirectory = true,
                });
            }

            foreach (var file in Directory.EnumerateFiles(path.ToString()))
            {
                result.Add(new DirectoryEntry()
                {
                    Path = new Path(file),
                    IsDirectory = false,
                });
            }

            return result;
        }

        /// <summary>
        /// Delete the directory
        /// </summary>
        public void DeleteDirectory(Path path, bool recursive)
        {
            Directory.Delete(path.ToString(), recursive);
        }
    }
}
