// <copyright file="FileSystemState.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Utilities;
using System;
using System.Collections.Generic;

namespace Soup.Build.Generate
{
	public record FileId(uint value)
	{
		public static FileId Invalid => new FileId(uint.MaxValue);
	}

	/// <summary>
	/// The complete set of known files that tracking the active change state during execution
	/// </summary>
	internal class FileSystemState
	{
		/// <summary>
		/// Initializes a new instance of the <see cref="FileSystemState"/> class.
		/// </summary>
		public FileSystemState()
		{
			_maxFileId = new FileId(0);
			_files = new Dictionary<FileId, Path>();
			_fileLookup = new Dictionary<string, FileId>();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="FileSystemState"/> class.
		/// </summary>
		public FileSystemState(
			FileId maxFileId,
			Dictionary<FileId, Path> files)
		{
			_maxFileId = maxFileId;
			_files = files;
			_fileLookup = new Dictionary<string, FileId>();

			// Build up the reverse lookup for new files
			foreach (var file in _files)
			{
				_fileLookup.Add(file.Value.ToString(), file.Key);
			}
		}

		/// <summary>
		/// Get Files
		/// </summary>
		public IReadOnlyDictionary<FileId, Path> GetFiles()
		{
			return _files;
		}

		/// <summary>
		/// Get the max unique file id
		/// </summary>
		public FileId GetMaxFileId()
		{
			return _maxFileId;
		}

		/// <summary>
		/// Convert a set of file paths to file ids
		/// </summary>
		public IList<FileId> ToFileIds(IReadOnlyList<Path> files, Path workingDirectory)
		{
			var result = new List<FileId>();
			foreach (var file in files)
			{
				result.Add(ToFileId(file, workingDirectory));
			}

			return result;
		}

		/// <summary>
		/// Convert a file path to file id
		/// </summary>
		public FileId ToFileId(Path file, Path workingDirectory)
		{
			var absolutePath = file.HasRoot ? file : workingDirectory + file;
			return ToFileId(absolutePath);
		}

		public FileId ToFileId(Path file)
		{
			if (!file.HasRoot)
				throw new InvalidOperationException("File paths must be absolute to resolve to an id");

			// Check if the file is already known
			if (!TryFindFileId(file, out var result))
			{
				// Insert the new file
				_maxFileId = new FileId(_maxFileId.value + 1);
				result = _maxFileId;

				_files.Add(result, file);
				_fileLookup.Add(file.ToString(), result);
			}

			return result;
		}

		/// <summary>
		/// Find an file id
		/// </summary>
		public bool TryFindFileId(Path file, out FileId fileId)
		{
			if (_fileLookup.TryGetValue(file.ToString(), out var value))
            {
				fileId = value;
				return true;
			}
			else
            {
				fileId = new FileId(0);
				return false;
            }
		}

		/// <summary>
		/// Find a file path
		/// </summary>
		public Path GetFilePath(FileId fileId)
		{
			return _files[fileId];
		}

		// The maximum id that has been used for files
		// Used to ensure unique ids are generated accross the entire system
		private FileId _maxFileId;

		private Dictionary<FileId, Path> _files;
		private Dictionary<string, FileId> _fileLookup;
	}
}
