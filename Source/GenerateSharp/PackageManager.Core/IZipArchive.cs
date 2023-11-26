// <copyright file="IZipArchive.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;

namespace Soup.Build.PackageManager;

/// <summary>
/// The zip archive interface
/// Interface mainly used to allow for unit testing client code.
/// </summary>
public interface IZipArchive : IDisposable
{
	public void CreateEntryFromFile(Path sourceFileName, string entryName);
}
