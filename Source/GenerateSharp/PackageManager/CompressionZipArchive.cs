// <copyright file="CompressionZipArchive.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

using Opal;
using System.IO.Compression;

namespace Soup.Build.PackageManager;

internal class CompressionZipArchive : IZipArchive
{
	ZipArchive _archive;

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