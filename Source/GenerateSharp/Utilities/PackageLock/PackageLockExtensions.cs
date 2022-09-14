// <copyright file="PackageLockExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System;
using System.Threading.Tasks;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The package lock extensions
	/// </summary>
	public static class PackageLockExtensions
	{
		/// <summary>
		/// Attempt to load the package lock from file
		/// </summary>
		public static async Task<(bool IsSuccess, PackageLock Result)> TryLoadFromFileAsync(Path packageLockFile)
		{
			// Verify the requested file exists
			Log.Diag("Load Package Lock: " + packageLockFile.ToString());
			if (!System.IO.File.Exists(packageLockFile.ToString()))
			{
				Log.Info("Package Lock file does not exist.");
				return (false, new PackageLock());
			}

			// Open the file to read from
			using (var fileStream = System.IO.File.OpenRead(packageLockFile.ToString()))
			using (var reader = new System.IO.StreamReader(fileStream))
			{
				// Read the contents of the recipe file
				try
				{
					var result = SMLManager.Deserialize(
						await reader.ReadToEndAsync());

					var packageLock = new PackageLock(result);
					if (!packageLock.HasVersion() || packageLock.GetVersion() != 2)
					{
						Log.Info("Package Lock version is incorrect.");
						return (false, new PackageLock());
					}

					return (true, packageLock);
				}
				catch (Exception ex)
				{
					Log.Error($"Deserialize Threw: {ex.Message}");
					Log.Info("Failed to parse Package Lock.");
					return (false, new PackageLock());
				}
			}
		}

		/// <summary>
		/// Save the package lock to file
		/// </summary>
		public static async Task SaveToFileAsync(
			Path packageLockFile,
			PackageLock packageLock)
		{
			// Open the file to write to
			var file = LifetimeManager.Get<IFileSystem>().OpenWrite(packageLockFile, true);

			// Write the recipe to the file stream
			await SMLManager.SerializeAsync(packageLock.Table, file.GetOutStream());
		}
	}
}
