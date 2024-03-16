// <copyright file="IFileSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System;

/// <summary>
/// The file system interface
/// Interface mainly used to allow for unit testing client code.
/// </summary>
public interface IFileSystem
{
	/// <summary>
	/// Gets the current user profile directory.
	/// </summary>
	public Path GetUserProfileDirectory();

	/// <summary>
	/// Gets the current directory for the running processes.
	/// </summary>
	public Path GetCurrentDirectory();

	/// <summary>
	/// Gets a value indicating whether the directory/file exists.
	/// </summary>
	/// <param name="path">The path.</param>
	public bool Exists(Path path);

	/// <summary>
	/// Get the last write time of the file/directory
	/// TODO: This should be a better representation of datetime...
	/// </summary>
	/// <param name="path">The path.</param>
	public int GetLastWriteTime(Path path);

	/// <summary>
	/// Set the last write time of the file/directory
	/// TODO: This should be a better representation of datetime...
	/// </summary>
	/// <param name="path">The path.</param>
	/// <param name="value">The value.</param>
	public void SetLastWriteTime(Path path, int value);

	/// <summary>
	/// Open the requested file as a stream to read.
	/// </summary>
	/// <param name="path">The path.</param>
	public IInputFile OpenRead(Path path);

	/// <summary>
	/// Open the requested file as a stream to write.
	/// </summary>
	/// <param name="path">The path.</param>
	/// <param name="truncate">A value indicating if open should truncate file.</param>
	public IOutputFile OpenWrite(Path path, bool truncate);

	/// <summary>
	/// Rename the source file to the destination.
	/// </summary>
	/// <param name="source">The source path.</param>
	/// <param name="destination">The destination path.</param>
	public void Rename(Path source, Path destination);

	/// <summary>
	/// Copy the source file to the destination.
	/// </summary>
	/// <param name="source">The source path.</param>
	/// <param name="destination">The destination path.</param>
	public void CopyFile2(Path source, Path destination);

	/// <summary>
	/// Create the directory at the requested path.
	/// </summary>
	/// <param name="path">The path.</param>
	public void CreateDirectory2(Path path);

	/// <summary>
	/// Get the children of a directory.
	/// </summary>
	/// <param name="path">The path.</param>
	public IReadOnlyList<DirectoryEntry> GetChildren(Path path);

	/// <summary>
	/// Get the child directories of a directory.
	/// </summary>
	/// <param name="path">The path.</param>
	public IReadOnlyList<DirectoryEntry> GetChildDirectories(Path path);

	/// <summary>
	/// Get the child files of a directory.
	/// </summary>
	/// <param name="path">The path.</param>
	public IReadOnlyList<DirectoryEntry> GetChildFiles(Path path);

	/// <summary>
	/// Delete the directory.
	/// </summary>
	/// <param name="path">The path.</param>
	/// <param name="recursive">A value indicating if the delete is recursive.</param>
	public void DeleteDirectory(Path path, bool recursive);

	/// <summary>
	/// Delete the file.
	/// </summary>
	/// <param name="path">The path.</param>
	public void DeleteFile(Path path);
}
