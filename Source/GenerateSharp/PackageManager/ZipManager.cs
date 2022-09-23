// <copyright file="ZipManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

using Opal;
using System.IO.Compression;

namespace Soup.Build.PackageManager
{
	internal class ZipManager : IZipManager
	{
		public void ExtractToDirectory(Path sourceArchiveFileName, Path destinationDirectoryName)
		{
			ZipFile.ExtractToDirectory(sourceArchiveFileName.ToString(), destinationDirectoryName.ToString());
		}

		public ZipArchive OpenCreate(Path archivePath)
		{
			return ZipFile.Open(archivePath.ToString(), ZipArchiveMode.Create);
		}
	}
}
