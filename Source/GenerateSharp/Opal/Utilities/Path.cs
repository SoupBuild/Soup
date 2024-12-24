// <copyright file="Path.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Opal;

/// <summary>
/// A container for a path string
/// ROOT: The optional root of the path, if not present then the path is relative and must
/// start with either the RelativeParentDirectory or RelativeDirectory special symbols
///  '/' - Rooted in the current drive
///  'A-Z:' - Rooted in a letter drive (Windows Specific)
///  '//' - Server root.
/// </summary>
public class Path : IEquatable<Path>
{
	private const char DirectorySeparator = '/';

	private const char AlternateDirectorySeparator = '\\';

	private static char[] AllValidDirectorySeparators => ['/', '\\'];

	private const char LetterDriveSpecifier = ':';

	private const char FileExtensionSeparator = '.';

	private const string RelativeDirectory = ".";

	private const string RelativeParentDirectory = "..";

	private string value;
	private int rootEndLocation;
	private int fileNameStartLocation;

	/// <summary>
	/// Initializes a new instance of the <see cref="Path"/> class.
	/// </summary>
	public Path()
	{
		this.value = "./";
		this.rootEndLocation = -1;
		this.fileNameStartLocation = 2;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="Path"/> class.
	/// </summary>
	/// <param name="value">The value.</param>
	public Path(string value)
	{
		this.value = value;
		LoadDirect();
	}

	/// <summary>
	/// Gets a value indicating whether the path is empty.
	/// </summary>
	public bool IsEmpty => this.value == "./";

	/// <summary>
	/// Gets a value indicating whether the path has a root.
	/// </summary>
	public bool HasRoot => this.rootEndLocation >= 0;

	/// <summary>
	/// Gets a value indicating whether the path has a file name.
	/// </summary>
	public bool HasFileName => this.fileNameStartLocation < this.value.Length;

	/// <summary>
	/// Gets the file name.
	/// </summary>
	public string FileName
	{
		get
		{
			if (!this.HasFileName)
				throw new InvalidOperationException("No filename");

			// Use the start location to return the end of the value that is the filename
			return this.value[this.fileNameStartLocation..];
		}
	}

	/// <summary>
	/// Gets a value indicating whether the file name has a stem.
	/// </summary>
	public bool HasFileStem => this.HasFileName && !string.IsNullOrEmpty(this.FileStem);

	/// <summary>
	/// Gets the file name minus the extension.
	/// </summary>
	public string FileStem
	{
		get
		{
			// Everything before the last period is the stem
			var fileName = this.FileName;
			var lastSeparator = fileName.LastIndexOf(FileExtensionSeparator);
			if (lastSeparator != -1)
			{
				return fileName[..lastSeparator];
			}
			else
			{
				// Return the entire filename if no extension
				return fileName;
			}
		}
	}

	/// <summary>
	/// Gets a value indicating whether the file name has an extension.
	/// </summary>
	public bool HasFileExtension => this.HasFileName && !string.IsNullOrEmpty(this.FileExtension);

	/// <summary>
	/// Gets the file extension.
	/// </summary>
	public string FileExtension
	{
		get
		{
			// Everything after and including the last period is the extension
			var fileName = this.FileName;
			var lastSeparator = fileName.LastIndexOf(FileExtensionSeparator);
			return lastSeparator != -1 ? fileName[lastSeparator..] : string.Empty;
		}
	}

	/// <summary>
	/// Gets the path root.
	/// </summary>
	public string Root
	{
		get
		{
			if (this.rootEndLocation < 0)
				throw new InvalidOperationException("Cannot access root on path that has none");
			return this.value[..this.rootEndLocation];
		}
	}

	public static Path Parse(string value)
	{
		var result = new Path();
		result.ParsePath(value);
		return result;
	}

	public static bool operator ==(Path? lhs, Path? rhs)
	{
		return lhs is null ? rhs is null : lhs.Equals(rhs);
	}

	public static bool operator !=(Path? lhs, Path? rhs)
	{
		return !(lhs == rhs);
	}

	/// <summary>
	/// Comparison operator.
	/// </summary>
	public int CompareTo(Path other)
	{
		return string.Compare(this.value, other.value, StringComparison.Ordinal);
	}

	public IList<string> DecomposeDirectories()
	{
		return DecomposeDirectoriesString(GetDirectories());
	}

	/// <summary>
	/// Concatenate paths.
	/// </summary>
	/// <param name="lhs">The left hand side.</param>
	/// <param name="rhs">The right hand side.</param>
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
		if (lhs.HasFileName)
		{
			resultDirectories.Add(lhs.FileName);
		}

		var rhsDirectories = DecomposeDirectoriesString(rhs.GetDirectories());
		resultDirectories.AddRange(rhsDirectories);

		NormalizeDirectories(resultDirectories, lhs.HasRoot);

		// Set the state with the root from the LHS and the filename from the RHS
		var result = new Path();
		result.SetState(
			resultDirectories,
			lhs.HasRoot ? lhs.Root : null,
			rhs.HasFileName ? rhs.FileName : null);

		return result;
	}

	/// <summary>
	/// Gets the parent directory.
	/// </summary>
	public Path GetParent()
	{
		var result = new Path()
		{
			// Take the root from the left hand side
			rootEndLocation = this.rootEndLocation
		};

		// If there is a filename then return the directory
		// Otherwise return one less directory
		if (this.HasFileName)
		{
			// Pass along the path minus the filename
			result.value = this.value[..this.fileNameStartLocation];
			result.fileNameStartLocation = result.value.Length;
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
			result.SetState(
				directories,
				this.HasRoot ? this.Root : null,
				null);
		}

		return result;
	}

	/// <summary>
	/// Set the filename.
	/// </summary>
	/// <param name="value">The value.</param>
	public void SetFilename(string value)
	{
		// Build the new final string
		SetState(
			DecomposeDirectoriesString(GetDirectories()),
			this.HasRoot ? this.Root : null,
			value);
	}

	/// <summary>
	/// Set the file extension.
	/// </summary>
	/// <param name="value">The value.</param>
	public void SetFileExtension(string value)
	{
		// Build up the new filename and set the active state
		SetFilename($"{this.FileStem}{FileExtensionSeparator}{value}");
	}

	/// <summary>
	/// Get a path relative to the provided base.
	/// </summary>
	/// <param name="basePath">The base path.</param>
	public Path GetRelativeTo(Path basePath)
	{
		// If the root does not match then there is no way to get a relative path
		// simply return a copy of this path
		if ((basePath.HasRoot && this.HasRoot && basePath.Root != this.Root) ||
			(basePath.HasRoot ^ this.HasRoot))
		{
			return this;
		}

		// Force the base filenames as directories
		var baseDirectories = DecomposeDirectoriesString(basePath.GetDirectories());
		if (basePath.HasFileName)
		{
			baseDirectories.Add(basePath.FileName);
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
			null,
			this.HasFileName ? this.FileName : null);

		return result;
	}

	/// <summary>
	/// Equality operator.
	/// </summary>
	/// <param name="other">The right hand side.</param>
	public bool Equals(Path? other)
	{
		if (other is null)
			return false;
		return this.value == other.value;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as Path);
	}

	public override int GetHashCode()
	{
		return this.value.GetHashCode(StringComparison.Ordinal);
	}

	/// <summary>
	/// Convert to string.
	/// </summary>
	public override string ToString()
	{
		return this.value;
	}

	public string ToAlternateString()
	{
		// Replace all normal separators with the windows version
		var result = this.value.Replace(DirectorySeparator, AlternateDirectorySeparator);
		return result;
	}

	private static bool IsRelativeDirectory(string directory)
	{
		return directory is RelativeDirectory or RelativeParentDirectory;
	}

	private static List<string> DecomposeDirectoriesString(string value)
	{
		var current = 0;
		int next;
		var directories = new List<string>();
		while ((next = value.IndexOf(DirectorySeparator, current)) != -1)
		{
			var directory = value[current..next];
			if (!string.IsNullOrEmpty(directory))
			{
				directories.Add(directory);
			}

			current = next + 1;
		}

		// Ensure the last separator was at the end of the string
		return current != value.Length ?
			throw new InvalidOperationException("The directories string must end in a separator") :
			directories;
	}

	private static bool IsRoot(string value)
	{
		if (value.Length == 0)
		{
			// Empty value is root
			return true;
		}
		else if (value.Length == 2)
		{
			// Check for drive letter
			if (char.IsLetter(value[0]) && value[1] == LetterDriveSpecifier)
			{
				return true;
			}
		}

		return false;
	}

	/// <summary>
	/// Resolve any up directory tokens or empty (double separator) directories that are inside a path.
	/// </summary>
	private static void NormalizeDirectories(
		List<string> directories,
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
							directories.RemoveAt(i);
							directories.RemoveAt(i - 1);
							i -= 2;
						}
					}
				}
			}
		}
	}

	/// <summary>
	/// Helper that loads a string directly into the path value
	/// </summary>
	private void LoadDirect()
	{
		var firstAlternateDirectory = this.value.IndexOf(AlternateDirectorySeparator, 0);
		if (firstAlternateDirectory != -1)
			throw new ArgumentException("Debug check for windows ridiculous directory separator");

		var firstSeparator = this.value.IndexOf(DirectorySeparator, 0);
		if (firstSeparator == -1)
		{
			throw new ArgumentException("A path must have a directory separator");
		}

		var root = this.value[..firstSeparator];
		if (IsRoot(root))
		{
			// Absolute path
			this.rootEndLocation = firstSeparator;
		}
		else if (root is RelativeDirectory or RelativeParentDirectory)
		{
			// Relative path
			this.rootEndLocation = -1;
		}
		else
		{
			throw new ArgumentException($"Unknown directory root {root}");
		}

		// Check if has file name
		var lastSeparator = this.value.LastIndexOf(DirectorySeparator);
		if (lastSeparator != -1 && lastSeparator != this.value.Length - 1)
		{
			this.fileNameStartLocation = lastSeparator + 1;
		}
		else
		{
			this.fileNameStartLocation = this.value.Length;
		}
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
		bool hasRoot = root is not null;
		NormalizeDirectories(directories, hasRoot);

		// Rebuild the string value
		SetState(
			directories,
			root,
			fileName);
	}

	private static void DecomposeRawPathString(
		string value,
		List<string> directories,
		out string? root,
		out string? fileName)
	{
		root = null;
		fileName = null;

		var current = 0;
		int next;
		var isFirst = true;
		while ((next = value.IndexOfAny(AllValidDirectorySeparators, current)) != -1)
		{
			var directory = value[current..next];

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
			var directory = value[current..];

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

	/// <summary>
	/// Convert the components of the path into the string value.
	/// </summary>
	private void SetState(
		List<string> directories,
		string? root,
		string? fileName)
	{
		var stringBuilder = new StringBuilder();

		if (root is not null)
		{
			_ = stringBuilder.Append(root);
			_ = stringBuilder.Append(DirectorySeparator);
		}

		for (var i = 0; i < directories.Count; i++)
		{
			_ = stringBuilder.Append(directories[i]);
			_ = stringBuilder.Append(DirectorySeparator);
		}

		if (fileName is not null)
		{
			_ = stringBuilder.Append(fileName);
		}

		// Store the persistent state
		this.value = stringBuilder.ToString();

		this.rootEndLocation = root is not null ? root.Length : -1;
		this.fileNameStartLocation = fileName is not null ? this.value.Length - fileName.Length : this.value.Length;

	}

	private string GetDirectories()
	{
		if (this.rootEndLocation >= 0)
		{
			return this.value[this.rootEndLocation..this.fileNameStartLocation];
		}
		else
		{
			return this.value[..this.fileNameStartLocation];
		}
	}
}
