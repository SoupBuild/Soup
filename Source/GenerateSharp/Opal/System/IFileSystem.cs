// <copyright file="IFileSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Opal.System
{
	public struct DirectoryEntry
	{
		public Path Path;
		public bool IsDirectory;
		public ulong Size;
		public int AccessTime;
		public int CreateTime;
		public int ModifiedTime;

		// TODO: Make generic
		public int Attributes;
	}

	/// <summary>
	/// The file system interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	public abstract class IFileSystem
	{
		/// <summary>
		/// Gets the current active file system
		/// </summary>
		public static IFileSystem Current()
		{
			if (_current == null)
				throw new InvalidOperationException("No file system implementation registered.");
			return _current;
		}

		/// <summary>
		/// Register a new active file system
		/// </summary>
		public static void Register(IFileSystem? fileSystem)
		{
			_current = fileSystem;
		}

		/// <summary>
		/// Gets the current user profile directory
		/// </summary>
		public abstract Path GetUserProfileDirectory();

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		public abstract Path GetCurrentDirectory2();

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		public abstract bool Exists(Path path);

		/// <summary>
		/// Get the last write time of the file/directory
		/// TODO: This should be a better representation of datetime...
		/// </summary>
		public abstract int GetLastWriteTime(Path path);

		/// <summary>
		/// Set the last write time of the file/directory
		/// TODO: This should be a better representation of datetime...
		/// </summary>
		public abstract void SetLastWriteTime(Path path, int value);

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		public abstract IInputFile OpenRead(Path path, bool isBinary);

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		public abstract IOutputFile OpenWrite(Path path, bool isBinary);

		/// <summary>
		/// Rename the source file to the destination
		/// </summary>
		public abstract void Rename(Path source, Path destination);

		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		public abstract void CopyFile2(Path source, Path destination);

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		public abstract void CreateDirectory2(Path path);

		/// <summary>
		/// Get the children of a directory
		/// </summary>
		public abstract IReadOnlyList<DirectoryEntry> GetDirectoryChildren(Path path);

		/// <summary>
		/// Delete the directory
		/// </summary>
		public abstract void DeleteDirectory(Path path, bool recursive);

		private static IFileSystem? _current = null;
	}
}
