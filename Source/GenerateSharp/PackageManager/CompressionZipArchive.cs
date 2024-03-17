// <copyright file="CompressionZipArchive.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

using System.IO.Compression;
using Path = Opal.Path;

namespace Soup.Build.PackageManager;

internal sealed class CompressionZipArchive : IZipArchive
{
	private readonly ZipArchive _archive;

	public CompressionZipArchive(ZipArchive archive)
	{
		_archive = archive;
	}

	public void CreateEntryFromFile(Path sourceFileName, string entryName)
	{
		_ = _archive.CreateEntryFromFile(
			sourceFileName.ToString(),
			entryName);
	}

	public void Dispose()
	{
		_archive.Dispose();
	}
}