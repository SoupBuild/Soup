// <copyright file="CompressionZipManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

using System.IO.Compression;
using Path = Opal.Path;

namespace Soup.Build.PackageManager;

internal sealed class CompressionZipManager : IZipManager
{
	public void ExtractToDirectory(Path sourceArchiveFileName, Path destinationDirectoryName)
	{
		ZipFile.ExtractToDirectory(sourceArchiveFileName.ToString(), destinationDirectoryName.ToString());
	}

	public IZipArchive OpenCreate(Path archivePath)
	{
		return new CompressionZipArchive(
			ZipFile.Open(archivePath.ToString(), ZipArchiveMode.Create));
	}
}
