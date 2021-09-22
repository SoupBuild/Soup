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
			using (var reader = new System.IO.StreamReader(file.GetInStream()))
			{
				// Read the contents of the local user config file
				try
				{
					var result = ValueTableTomlUtilities.Deserialize(
						localUserConfigFile,
						await reader.ReadToEndAsync());
					return (true, new LocalUserConfig(result.Table, result.Root));
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
			// Open the file to write to
			var file = LifetimeManager.Get<IFileSystem>().OpenWrite(configFile, true);

			// Serialize the contents of the recipe
			var documentSyntax = config.MirrorSyntax;
			if (documentSyntax == null)
				throw new ArgumentException("The provided config does not have a mirrored syntax tree.", nameof(config));

			// Write the recipe to the file stream
			await ValueTableTomlUtilities.SerializeAsync(documentSyntax, file.GetOutStream());
		}
	}
}
