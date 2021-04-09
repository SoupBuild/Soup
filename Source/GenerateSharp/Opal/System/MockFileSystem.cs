// <copyright file="MockFileSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;

namespace Opal.System
{
	/// <summary>
	/// The mock file system
	/// TODO: Move into test project
	/// </summary>
	public class MockFileSystem : IFileSystem
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='MockFileSystem'/> class.
		/// </summary>
		public MockFileSystem()
		{
			_requests = new List<string>();
			_files = new Dictionary<Path, MockFile>();
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		public IReadOnlyList<string> GetRequests()
		{
			return _requests;
		}

		/// <summary>
		/// Create a test file
		/// </summary>
		public void CreateMockFile(Path path, MockFile file)
		{
			_files.Add(path, file);
		}

		/// <summary>
		/// Create a test file
		/// </summary>
		public MockFile GetMockFile(Path path)
		{
			if (_files.TryGetValue(path, out var file))
			{
				return file;
			}
			else
			{
				throw new InvalidOperationException($"Cannot find file: {path}");
			}
		}

		/// <summary>
		/// Gets the current user profile directory
		/// </summary>
		public override Path GetUserProfileDirectory()
		{
			_requests.Add("GetCurrentDirectory");

			return new Path("C:/Users/Me/");
		}

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		public override Path GetCurrentDirectory2()
		{
			_requests.Add("GetCurrentDirectory");

			return new Path("C:/Current/");
		}

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		public override bool Exists(Path path)
		{
			_requests.Add($"Exists: {path}");

			return _files.ContainsKey(path);
		}

		/// <summary>
		/// Get the last write time of the file/directory
		/// </summary>
		public override int GetLastWriteTime(Path path)
		{
			_requests.Add($"GetLastWriteTime: {path}");

			if (_files.TryGetValue(path, out var file))
			{
				return file.LastWriteTime;
			}
			else
			{
				throw new InvalidOperationException($"Cannot find file for last write time: {path}");
			}
		}

		/// <summary>
		/// Set the last write time of the file/directory
		/// </summary>
		public override void SetLastWriteTime(Path path, int value)
		{
			_requests.Add($"SetLastWriteTime: {path}");
		}

		/// <summary>
		/// Open the requested file as a stream to read
		/// </summary>
		public override IInputFile OpenRead(Path path, bool isBinary)
		{
			if (isBinary)
			{
				_requests.Add($"OpenReadBinary: {path}");
			}
			else
			{
				_requests.Add($"OpenRead: {path}");
			}

			if (_files.TryGetValue(path, out var file))
			{
				return new MockInputFile(file);
			}
			else
			{
				throw new InvalidOperationException($"Cannot open read: {path}");
			}
		}

		/// <summary>
		/// Open the requested file as a stream to write
		/// </summary>
		public override IOutputFile OpenWrite(Path path, bool isBinary)
		{
			if (isBinary)
			{
				_requests.Add($"OpenWriteBinary: {path}");
			}
			else
			{
				_requests.Add($"OpenWrite: {path}");
			}

			if (_files.TryGetValue(path, out var file))
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
				_files.Add(path, insert);
				return new MockOutputFile(insert);
			}
		}

		/// <summary>
		/// Rename the source file to the destination
		/// </summary>
		public override void Rename(Path source, Path destination)
		{
			_requests.Add($"Rename: [{source}] -> [{destination}]");
		}

		/// <summary>
		/// Copy the source file to the destination
		/// </summary>
		public override void CopyFile2(Path source, Path destination)
		{
			_requests.Add($"CopyFile: [{source}] -> [{destination}]");
		}

		/// <summary>
		/// Create the directory at the requested path
		/// </summary>
		public override void CreateDirectory2(Path path)
		{
			_requests.Add($"CreateDirectory: {path}");
		}

		/// <summary>
		/// Get the children of a directory
		/// </summary>
		public override IReadOnlyList<DirectoryEntry> GetDirectoryChildren(Path path)
		{
			_requests.Add($"GetDirectoryChildren: {path}");

			var result = new List<DirectoryEntry>();
			return result;
		}

		/// <summary>
		/// Delete the directory
		/// </summary>
		public override void DeleteDirectory(Path path, bool recursive)
		{
			if (recursive)
				_requests.Add($"DeleteDirectoryRecursive: {path}");
			else
				_requests.Add($"DeleteDirectory: {path}");
		}

		private List<string> _requests;
		private Dictionary<Path, MockFile> _files;
	}
}
