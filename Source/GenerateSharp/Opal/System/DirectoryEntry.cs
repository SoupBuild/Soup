// <copyright file="DirectoryEntry.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System;

/// <summary>
/// The directory entry.
/// </summary>
public class DirectoryEntry
{
	public Path Path { get; set; } = new Path();

	public bool IsDirectory { get; set; }

	public ulong Size { get; set; }

	public int AccessTime { get; set; }

	public int CreateTime { get; set; }

	public int ModifiedTime { get; set; }

	// TODO: Make generic
	public int Attributes { get; set; }
}
