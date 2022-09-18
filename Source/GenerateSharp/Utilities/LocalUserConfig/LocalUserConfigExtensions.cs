// <copyright file="LocalUserConfigExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System;
using System.Threading.Tasks;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The local user config extensions
	/// </summary>
	public class LocalUserConfigExtensions
	{
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		public static async Task<(bool IsSuccess, LocalUserConfig Result)> TryLoadLocalUserConfigFromFileAsync(
			Path localUserConfigFile)
		{
			// Verify the requested file exists
			Log.Diag("Load Local User Config: " + localUserConfigFile.ToString());
			if (!LifetimeManager.Get<IFileSystem>().Exists(localUserConfigFile))
			{
				Log.Warning("Local User Config file does not exist.");
				return (false, new LocalUserConfig());
			}

			// Open the file to read from
			var file = LifetimeManager.Get<IFileSystem>().OpenRead(localUserConfigFile);

			// Open the file to read from
			using (var reader = new System.IO.StreamReader(file.GetInStream(), null, true, -1, true))
			{
				// Read the contents of the local user config file
				try
				{
					var result = SMLManager.Deserialize(
						await reader.ReadToEndAsync());
					return (true, new LocalUserConfig(result));
				}
				catch (Exception ex)
				{
					Log.Error("Deserialize Threw: " + ex.Message);
					Log.Info("Failed to parse local user config.");
					return (false, new LocalUserConfig());
				}
			}
		}

		/// <summary>
		/// Save the config to file
		/// </summary>
		public static async Task SaveToFileAsync(
			Path configFile,
			LocalUserConfig config)
		{
			// Ensure the parent folder exists
			var folder = configFile.GetParent();
			if (!LifetimeManager.Get<IFileSystem>().Exists(folder))
			{
				Log.Info($"Creating directory {folder}");
				LifetimeManager.Get<IFileSystem>().CreateDirectory2(folder);
			}

			// Open the file to write to
			var file = LifetimeManager.Get<IFileSystem>().OpenWrite(configFile, true);

			// Write the recipe to the file stream
			await SMLManager.SerializeAsync(
				config.GetDocument(),
				file.GetOutStream());
		}
	}
}
