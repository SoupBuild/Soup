// <copyright file="Path.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// A container for a path string
	/// ROOT: The optional root of the path, if not present then the path is relative and must
	/// start with either the RelativeParentDirectory or RelativeDirectory special symbols
	///  '/' - Rooted in the current drive
	///  'A-Z:' - Rooted in a letter drive (Windows Specific)
	///  '//' - Server root
	/// </summary>
	public class Path : IEquatable<Path>
	{
		private static char DirectorySeparator => '/';
		private static char AlternateDirectorySeparator => '\\';
		private static char[] AllValidDirectorySeparators => new char[] { '/', '\\' };
		private static char LetterDriveSpecifier => ':';
		private static char FileExtensionSeparator => '.';
		private static string RelativeDirectory => ".";
		private static string RelativeParentDirectory => "..";

		/// <summary>
		/// Initializes a new instance of the <see cref="Path"/> class.
		/// </summary>
		public Path()
		{
			_value = string.Empty;
			SetState(new List<string>() { RelativeDirectory }, string.Empty, string.Empty);
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Path"/> class.
		/// </summary>
		public Path(string value)
		{
			_value = string.Empty;
			ParsePath(value);
		}

		/// <summary>
		/// Gets a value indicating whether the path is empty
		/// </summary>
		public bool IsEmpty => _value == "./";

		/// <summary>
		/// Gets a value indicating whether the path has a root
		/// </summary>
		public bool HasRoot => _rootEndLocation > 0;

		/// <summary>
		/// Gets the path root
		/// </summary>
		public string GetRoot()
		{
			return _value.Substring(0, _rootEndLocation);
		}

		/// <summary>
		/// Gets the parent directory
		/// </summary>
		public Path GetParent()
		{
			var result = new Path();

			// Take the root from the left hand side
			result._rootEndLocation = _rootEndLocation;

			// If there is a filename then return the directory
			// Otherwise return one less directory
			if (HasFileName())
			{
				// Pass along the path minus the filename
				result._value = _value.Substring(0, _fileNameStartLocation);
				result._fileNameStartLocation = result._value.Length;
			}
			else
			{
				// Pull apart the directories and remove the last one
				// TODO: This can be done in place and then a substring returned for perf gains
				var directories = DecomposeDirectoriesString(GetDirectories());
				if (directories.Count == 0)
				{
					// No-op when at the root
				}
				else if (directories.Count == 1 && directories[0] == RelativeDirectory)
				{
					// If this is only a relative folder symbol then replace with the parent symbol
					directories[0] = RelativeParentDirectory;
				}
				else if (directories.Last() == RelativeParentDirectory)
				{
					// If this is entirely parent directories then add one more
					directories.Add(RelativeParentDirectory);
				}
				else
				{
					// Otherwise pop off the top level folder
					directories.RemoveAt(directories.Count - 1);
				}

				// Set the state of the result path
				result.SetState(directories, GetRoot(), string.Empty);
			}

			return result;
		}

		/// <summary>
		/// Gets a value indicating whether the path has a file name
		/// </summary>
		public bool HasFileName()
		{
			return _fileNameStartLocation < _value.Length;
		}

		/// <summary>
		/// Gets the file name
		/// </summary>
		public string GetFileName()
		{
			// Use the start location to return the end of the value that is the filename
			return _value.Substring(
				_fileNameStartLocation,
				_value.Length - _fileNameStartLocation);
		}

		/// <summary>
		/// Gets a value indicating whether the file name has an stem
		/// </summary>
		public bool HasFileStem()
		{
			return !string.IsNullOrEmpty(GetFileStem());
		}

		/// <summary>
		/// Gets the file name minus the extension
		/// </summary>
		public string GetFileStem()
		{
			// Everything before the last period is the stem
			var fileName = GetFileName();
			var lastSeparator = fileName.LastIndexOf(FileExtensionSeparator);
			if (lastSeparator != -1)
			{
				return fileName.Substring(0, lastSeparator);
			}
			else
			{
				// Return the entire filename if no extension
				return fileName;
			}
		}

		/// <summary>
		/// Gets a value indicating whether the file name has an extension
		/// </summary>
		public bool HasFileExtension()
		{
			return !string.IsNullOrEmpty(GetFileExtension());
		}

		/// <summary>
		/// Gets the file extension
		/// </summary>
		public string GetFileExtension()
		{
			// Everything after and including the last period is the extension
			var fileName = GetFileName();
			var lastSeparator = fileName.LastIndexOf(FileExtensionSeparator);
			if (lastSeparator != -1)
			{
				return fileName.Substring(lastSeparator);
			}
			else
			{
				return string.Empty;
			}
		}

		/// <summary>
		/// Set the filename
		/// </summary>
		public void SetFilename(string value)
		{
			// Build the new final string
			SetState(
				DecomposeDirectoriesString(GetDirectories()),
				GetRoot(),
				value);
		}

		/// <summary>
		/// Set the file extension
		/// </summary>
		public void SetFileExtension(string value)
		{
			// Build up the new filename and set the active state
			SetFilename($"{GetFileStem()}{FileExtensionSeparator}");
		}

		/// <summary>
		/// Get a path relative to the provided base
		/// </summary>
		public Path GetRelativeTo(Path basePath)
		{
			// If the root does not match then there is no way to get a relative path
			// simply return a copy of this path
			if ((basePath.HasRoot && HasRoot && basePath.GetRoot() != GetRoot()) ||
				(basePath.HasRoot ^ HasRoot))
			{
				return this;
			}

			// Force the base filenames as directories
			var baseDirectories = DecomposeDirectoriesString(basePath.GetDirectories());
			if (basePath.HasFileName())
			{
				baseDirectories.Add(basePath.GetFileName());
			}

			// Determine how many of the directories match
			var directories = DecomposeDirectoriesString(GetDirectories());
			var minDirectories = Math.Min(baseDirectories.Count, directories.Count);
			int countMatching = 0;
			for (var i = 0; i < minDirectories; i++)
			{
				if (baseDirectories[i] != directories[i])
				{
					break;
				}

				countMatching++;
			}

			// Add in up directories for any not matching in the base
			var resultDirectories = new List<string>();
			if (countMatching == baseDirectories.Count)
			{
				// Start with a single relative directory when no up directories required
				resultDirectories.Add(RelativeDirectory);
			}
			else
			{
				for (var i = countMatching; i < baseDirectories.Count; i++)
				{
					resultDirectories.Add(RelativeParentDirectory);
				}
			}

			// Copy over the remaining entities from the target path
			for (var i = countMatching; i < directories.Count; i++)
			{
				resultDirectories.Add(directories[i]);
			}

			// Set the result path with no root
			var result = new Path();
			result.SetState(
				resultDirectories,
				string.Empty,
				GetFileName());

			return result;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		public bool Equals(Path? rhs)
		{
			if (ReferenceEquals(rhs, null))
				return false;
			return _value == rhs._value;
		}

		public override bool Equals(object? rhs)
		{
			return Equals(rhs as Path);
		}

		public override int GetHashCode()
		{
			return _value.GetHashCode();
		}

		public static bool operator ==(Path lhs, Path rhs)
		{
			if (ReferenceEquals(lhs, null))
				return ReferenceEquals(rhs, null);
			return lhs.Equals(rhs);
		}

		public static bool operator !=(Path lhs, Path rhs)
		{
			return !(lhs == rhs);
		}

		/// <summary>
		/// Comparison operator
		/// </summary>
		public static bool operator <(Path lhs, Path rhs)
		{
			return string.Compare(lhs._value, rhs._value, StringComparison.Ordinal) < 0;
		}

		public static bool operator >(Path lhs, Path rhs)
		{
			return string.Compare(lhs._value, rhs._value, StringComparison.Ordinal) > 0;
		}

		/// <summary>
		/// Concatenate paths
		/// </summary>
		public static Path operator +(Path lhs, Path rhs)
		{
			if (rhs.HasRoot)
			{
				throw new ArgumentException(
					"Cannot combine a rooted path on the right hand side: " + rhs.ToString());
			}

			// Combine the directories
			var resultDirectories = DecomposeDirectoriesString(lhs.GetDirectories());

			// Convert the left hand side filename to a directory
			if (lhs.HasFileName())
			{
				resultDirectories.Add(lhs.GetFileName());
			}

			var rhsDirectories = DecomposeDirectoriesString(rhs.GetDirectories());
			resultDirectories.AddRange(rhsDirectories);

			NormalizeDirectories(resultDirectories, lhs.HasRoot);

			// Set the state with the root from the LHS and the filename from the RHS
			var result = new Path();
			result.SetState(
				resultDirectories,
				lhs.GetRoot(),
				rhs.GetFileName());

			return result;
		}

		/// <summary>
		/// Convert to string
		/// </summary>
		public override string ToString()
		{
			return _value;
		}

		public string ToAlternateString()
		{
			// Replace all normal separators with the windows version
			var result = _value.Replace(DirectorySeparator, AlternateDirectorySeparator);
			return result;
		}

		private void ParsePath(string value)
		{
			// Break out the individual components of the path
			var directories = new List<string>();
			DecomposeRawPathString(
				value,
				directories,
				out var root,
				out var fileName);

			// Normalize any unnecessary directories in the raw path
			bool hasRoot = !string.IsNullOrEmpty(root);
			NormalizeDirectories(directories, hasRoot);

			// Rebuild the string value
			SetState(
				directories,
				root,
				fileName);
		}

		private void DecomposeRawPathString(
			string value,
			IList<string> directories,
			out string root,
			out string fileName)
		{
			root = string.Empty;
			fileName = string.Empty;

			var current = 0;
			var next = 0;
			var isFirst = true;
			while ((next = value.IndexOfAny(AllValidDirectorySeparators, current)) != -1)
			{
				var directory = value.Substring(current, next - current);

				// Check if the first entry is a root
				if (isFirst)
				{
					if (IsRoot(directory))
					{
						root = directory;
					}
					else
					{
						// Ensure that the unrooted path starts with a relative symbol
						if (!IsRelativeDirectory(directory))
						{
							directories.Add(RelativeDirectory);
						}

						directories.Add(directory);
					}

					isFirst = false;
				}
				else
				{
					directories.Add(directory);
				}

				current = next + 1;
			}

			// Check if there are characters beyond the last separator
			if (current != value.Length)
			{
				var directory = value.Substring(current);

				// Check if still on the first entry
				// Could be empty root or single filename
				if (isFirst)
				{
					if (IsRoot(directory))
					{
						root = directory;
					}
					else
					{
						// Ensure that the unrooted path starts with a relative symbol
						if (!IsRelativeDirectory(directory))
						{
							directories.Add(RelativeDirectory);
						}

						fileName = directory;
					}

					isFirst = false;
				}
				else
				{
					fileName = directory;
				}
			}

			// If we saw nothing then add a single relative directory
			if (isFirst)
			{
				directories.Add(RelativeDirectory);
			}
		}

		private bool IsRelativeDirectory(string directory)
		{
			return directory == RelativeDirectory || directory == RelativeParentDirectory;
		}

		private static List<string> DecomposeDirectoriesString(string value)
		{
			var current = 0;
			var next = 0;
			var directories = new List<string>();
			while ((next = value.IndexOf(DirectorySeparator, current)) != -1)
			{
				var directory = value.Substring(current, next - current);
				if (!string.IsNullOrEmpty(directory))
				{
					directories.Add(directory);
				}

				current = next + 1;
			}

			// Ensure the last separator was at the end of the string
			if (current != value.Length)
			{
				throw new InvalidOperationException("The directories string must end in a separator");
			}

			return directories;
		}

		private bool IsRoot(string value)
		{
			// Check for drive letter
			if (value.Length == 2)
			{
				if (char.IsLetter(value[0]) && value[1] == LetterDriveSpecifier)
				{
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Resolve any up directory tokens or empty (double separator) directories that are inside a path
		/// </summary>
		private static void NormalizeDirectories(
			IList<string> directories,
			bool hasRoot)
		{
			// Remove as many up directories as we can
			for (var i = 0; i < directories.Count; i++)
			{
				// Remove empty directories (double separator) or relative directories if rooted or not at start
				if (string.IsNullOrEmpty(directories[i]) ||
					((hasRoot || i != 0) && directories[i] == RelativeDirectory))
				{
					directories.RemoveAt(i);
					i -= 1;
				}
				else
				{
					// Check if we can combine any parent directories
					// Allow the first directory to remain a parent
					if (i != 0)
					{
						// Remove a parent directory if possible
						if (directories[i] == RelativeParentDirectory &&
							directories[i - 1] != RelativeParentDirectory)
						{
							// If the previous is a relative then just replace it
							if (directories[i - 1] == RelativeDirectory)
							{
								directories.RemoveAt(i - 1);
								i -= 1;
							}
							else
							{
								// Remove the directories and move back
								directories.RemoveAt(i - 1);
								i -= 2;
							}
						}
					}
				}
			}
		}

		/// <summary>
		/// Convert the components of the path into the string value
		/// </summary>
		private void SetState(
			IList<string> directories,
			string root,
			string fileName)
		{
			var stringBuilder = new StringBuilder();

			if (!string.IsNullOrEmpty(root))
			{
				stringBuilder.Append(root);
				stringBuilder.Append(DirectorySeparator);
			}

			for (var i = 0; i < directories.Count; i++)
			{
				stringBuilder.Append(directories[i]);
				stringBuilder.Append(DirectorySeparator);
			}

			if (!string.IsNullOrEmpty(fileName))
			{
				stringBuilder.Append(fileName);
			}

			// Store the persistant state
			_value = stringBuilder.ToString();
			_rootEndLocation = root.Length;
			_fileNameStartLocation = _value.Length - fileName.Length;
		}

		private string GetDirectories()
		{
			return _value.Substring(
				_rootEndLocation,
				_fileNameStartLocation - _rootEndLocation);
		}

		private string _value;
		private int _rootEndLocation;
		private int _fileNameStartLocation;
	}
}
