// <copyright file="IFileSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
	public interface IFileSystem
	{
		/// <summary>
		/// Gets the current user profile directory
		/// </summary>
		public Path GetUserProfileDirectory();

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		public Path GetCurrentDirectory2();

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		public bool Exists(Path path);

		/// <summary>
		/// Get the last write time of the file/directory
		/// TODO: This should be a better representation of datetime...
		/// </summary>
		public int GetLastWriteTime(Path path);

		/// <summary>
		/// Set the last write time of the file/directory
		/// TODO: This should be a better representation of datetime...
		/// </summary>
		public void SetLastWriteTime(Path path, int value);

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		public IInputFile OpenRead(Path path, bool isBinary);

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		public IOutputFile OpenWrite(Path path, bool isBinary);

		/// <summary>
		/// Rename the source file to the destination
		/// </summary>
		public void Rename(Path source, Path destination);

		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		public void CopyFile2(Path source, Path destination);

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		public void CreateDirectory2(Path path);

		/// <summary>
		/// Get the children of a directory
		/// </summary>
		public IReadOnlyList<DirectoryEntry> GetDirectoryChildren(Path path);

		/// <summary>
		/// Delete the directory
		/// </summary>
		public void DeleteDirectory(Path path, bool recursive);
	}
}
