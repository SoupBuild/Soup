// <copyright file="CompressionZipArchive.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

using Opal;
using System.IO.Compression;

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