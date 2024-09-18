// <copyright file="MockFileSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System;

/// <summary>
/// The mock file system
/// TODO: Move into test project.
/// </summary>
public class MockFileSystem : IFileSystem
{
	private readonly List<string> _requests;
	private readonly Dictionary<Path, MockFile> _files;
	private readonly Dictionary<Path, IReadOnlyList<DirectoryEntry>> _directoryChildren;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockFileSystem'/> class.
	/// </summary>
	public MockFileSystem()
	{
		requests = [];
		files = [];
		directoryChildren = [];
	}

	/// <summary>
	/// Get the load requests.
	/// </summary>
	public IReadOnlyList<string> Requests => requests;

	/// <summary>
	/// Create a test file.
	/// </summary>
	/// <param name="path">The path.</param>
	/// <param name="file">The file.</param>
	public void CreateMockFile(Path path, MockFile file)
	{
		files.Add(path, file);
	}

	/// <summary>
	/// Create a test file.
	/// </summary>
	/// <param name="path">The path.</param>
	public MockFile GetMockFile(Path path)
	{
		if (files.TryGetValue(path, out var file))
		{
			return file;
		}
		else
		{
			throw new InvalidOperationException($"Cannot find file: {path}");
		}
	}

	public void RegisterChildren(Path path, IReadOnlyList<DirectoryEntry> response)
	{
		directoryChildren.Add(path, response);
	}

	/// <summary>
	/// Gets the current user profile directory.
	/// </summary>
	public Path GetUserProfileDirectory()
	{
		requests.Add("GetUserProfileDirectory");

		return new Path("C:/Users/Me/");
	}

	/// <summary>
	/// Gets the current directory for the running processes.
	/// </summary>
	public Path GetCurrentDirectory()
	{
		requests.Add("GetCurrentDirectory");

		return new Path("C:/Current/");
	}

	/// <summary>
	/// Gets a value indicating whether the directory/file exists.
	/// </summary>
	/// <param name="path">The path.</param>
	public bool Exists(Path path)
	{
		requests.Add($"Exists: {path}");

		return files.ContainsKey(path) || directoryChildren.ContainsKey(path);
	}

	/// <summary>
	/// Get the last write time of the file/directory.
	/// </summary>
	/// <param name="path">The path.</param>
	public int GetLastWriteTime(Path path)
	{
		requests.Add($"GetLastWriteTime: {path}");

		if (files.TryGetValue(path, out var file))
		{
			return file.LastWriteTime;
		}
		else
		{
			throw new InvalidOperationException($"Cannot find file for last write time: {path}");
		}
	}

	/// <summary>
	/// Set the last write time of the file/directory.
	/// </summary>
	/// <param name="path">The path.</param>
	/// <param name="value">The value.</param>
	public void SetLastWriteTime(Path path, int value)
	{
		requests.Add($"SetLastWriteTime: {path}");
	}

	/// <summary>
	/// Open the requested file as a stream to read.
	/// </summary>
	/// <param name="path">The path.</param>
	public IInputFile OpenRead(Path path)
	{
		requests.Add($"OpenRead: {path}");

		if (files.TryGetValue(path, out var file))
		{
			return new MockInputFile(file);
		}
		else
		{
			throw new InvalidOperationException($"Cannot open read: {path}");
		}
	}

	/// <summary>
	/// Open the requested file as a stream to write.
	/// </summary>
	/// <param name="path">The path.</param>
	/// <param name="truncate">A value indicating if open should truncate.</param>
	public IOutputFile OpenWrite(Path path, bool truncate)
	{
		if (truncate)
		{
			requests.Add($"OpenWriteTruncate: {path}");
		}
		else
		{
			requests.Add($"OpenWrite: {path}");
		}

		if (files.TryGetValue(path, out var file))
		{
			// Reset the existing content offset and return it.
			var content = file.Content;
			content.SetLength(0);
			return new MockOutputFile(file);
		}
		else
		{
			// Create the file if it does not exist
			var insert = new MockFile();
			files.Add(path, insert);
			return new MockOutputFile(insert);
		}
	}

	/// <summary>
	/// Rename the source file to the destination.
	/// </summary>
	/// <param name="source">The source.</param>
	/// <param name="destination">The destination.</param>
	public void Rename(Path source, Path destination)
	{
		requests.Add($"Rename: [{source}] -> [{destination}]");

		// Create a fake destination directory
		this.RegisterChildren(destination, []);
	}

	/// <summary>
	/// Copy the source file to the destination.
	/// </summary>
	/// <param name="source">The source.</param>
	/// <param name="destination">The destination.</param>
	public void CopyFile2(Path source, Path destination)
	{
		requests.Add($"CopyFile: [{source}] -> [{destination}]");
	}

	/// <summary>
	/// Create the directory at the requested path.
	/// </summary>
	/// <param name="path">The path.</param>
	public void CreateDirectory2(Path path)
	{
		requests.Add($"CreateDirectory: {path}");
	}

	/// <summary>
	/// Get the children of a directory.
	/// </summary>
	/// <param name="path">The path.</param>
	public IReadOnlyList<DirectoryEntry> GetChildren(Path path)
	{
		requests.Add($"GetChildren: {path}");

		if (directoryChildren.TryGetValue(path, out var children))
		{
			// Reset the existing content offset and return it.
			var result = children.ToList();
			return result;
		}
		else
		{
			var result = new List<DirectoryEntry>();
			return result;
		}
	}

	/// <summary>
	/// Get the children of a directory.
	/// </summary>
	/// <param name="path">The path.</param>
	public IReadOnlyList<DirectoryEntry> GetChildDirectories(Path path)
	{
		requests.Add($"GetChildDirectories: {path}");

		if (directoryChildren.TryGetValue(path, out var children))
		{
			// Reset the existing content offset and return it.
			var result = children
				.Where(value => value.IsDirectory)
				.ToList();
			return result;
		}
		else
		{
			var result = new List<DirectoryEntry>();
			return result;
		}
	}

	/// <summary>
	/// Get the children of a directory.
	/// </summary>
	/// <param name="path">The path.</param>
	public IReadOnlyList<DirectoryEntry> GetChildFiles(Path path)
	{
		requests.Add($"GetChildFiles: {path}");

		if (directoryChildren.TryGetValue(path, out var children))
		{
			// Reset the existing content offset and return it.
			var result = children
				.Where(value => !value.IsDirectory)
				.ToList();
			return result;
		}
		else
		{
			var result = new List<DirectoryEntry>();
			return result;
		}
	}

	/// <summary>
	/// Delete the directory.
	/// </summary>
	/// <param name="path">The path.</param>
	/// <param name="recursive">A value indicating if the delete is recursive.</param>
	public void DeleteDirectory(Path path, bool recursive)
	{
		if (recursive)
			requests.Add($"DeleteDirectoryRecursive: {path}");
		else
			requests.Add($"DeleteDirectory: {path}");
	}

	/// <summary>
	/// Delete the file.
	/// </summary>
	/// <param name="path">The path.</param>
	public void DeleteFile(Path path)
	{
		requests.Add($"DeleteFile: {path}");
	}
}