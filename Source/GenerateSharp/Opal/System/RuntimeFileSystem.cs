// <copyright file="RuntimeFileSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Runtime.InteropServices;

namespace Opal.System;

/// <summary>
/// The runtime library file system.
/// </summary>
public class RuntimeFileSystem : IFileSystem
{
	/// <summary>
	/// Initializes a new instance of the <see cref='RuntimeFileSystem'/> class.
	/// </summary>
	public RuntimeFileSystem()
	{
	}

	public Path GetUserProfileDirectory()
	{
		string? userProfileFolder = (RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
			? Environment.GetEnvironmentVariable("USERPROFILE")
			: Environment.GetEnvironmentVariable("HOME")) ??
			throw new InvalidOperationException("Unable to retrieve user profile");
		return Path.Parse($"{userProfileFolder}\\");
	}

	public Path GetCurrentDirectory()
	{
		throw new NotImplementedException();
	}

	public bool Exists(Path path)
	{
		return global::System.IO.File.Exists(path.ToString()) ||
			global::System.IO.Directory.Exists(path.ToString());
	}

	public int GetLastWriteTime(Path path)
	{
		throw new NotImplementedException();
	}

	public void SetLastWriteTime(Path path, int value)
	{
		throw new NotImplementedException();
	}

	public IInputFile OpenRead(Path path)
	{
		return new RuntimeInputFile(
			global::System.IO.File.OpenRead(path.ToString()));
	}

	public IOutputFile OpenWrite(Path path, bool truncate)
	{
		var mode = truncate ? global::System.IO.FileMode.Create : global::System.IO.FileMode.OpenOrCreate;
		return new RuntimeOutputFile(
			global::System.IO.File.Open(path.ToString(), mode, global::System.IO.FileAccess.Write));
	}

	public virtual void Rename(Path source, Path destination)
	{
		global::System.IO.Directory.Move(
			source.ToString(),
			destination.ToString());
	}

	public void CopyFile2(Path source, Path destination)
	{
		throw new NotImplementedException();
	}

	public void CreateDirectory2(Path path)
	{
		_ = global::System.IO.Directory.CreateDirectory(path.ToString());
	}

	public IReadOnlyList<DirectoryEntry> GetChildren(Path path)
	{
		var result = new List<DirectoryEntry>();
		foreach (var directory in global::System.IO.Directory.EnumerateDirectories(path.ToString()))
		{
			result.Add(new DirectoryEntry()
			{
				Path = new Path(directory),
				IsDirectory = true,
			});
		}

		foreach (var file in global::System.IO.Directory.EnumerateFiles(path.ToString()))
		{
			result.Add(new DirectoryEntry()
			{
				Path = new Path(file),
				IsDirectory = false,
			});
		}

		return result;
	}

	public IReadOnlyList<DirectoryEntry> GetChildDirectories(Path path)
	{
		var result = new List<DirectoryEntry>();
		foreach (var directory in global::System.IO.Directory.EnumerateDirectories(path.ToString()))
		{
			result.Add(new DirectoryEntry()
			{
				Path = new Path($"{directory}/"),
				IsDirectory = true,
			});
		}

		return result;
	}

	public IReadOnlyList<DirectoryEntry> GetChildFiles(Path path)
	{
		var result = new List<DirectoryEntry>();
		foreach (var directory in global::System.IO.Directory.EnumerateFiles(path.ToString()))
		{
			result.Add(new DirectoryEntry()
			{
				Path = new Path(directory),
				IsDirectory = true,
			});
		}

		return result;
	}

	public void DeleteDirectory(Path path, bool recursive)
	{
		global::System.IO.Directory.Delete(path.ToString(), recursive);
	}

	public void DeleteFile(Path path)
	{
		global::System.IO.File.Delete(path.ToString());
	}
}
