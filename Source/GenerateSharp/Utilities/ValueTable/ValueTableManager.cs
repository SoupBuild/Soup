// <copyright file="ValueTableManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using System;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The Value Table state manager
	/// </summary>
	public static class ValueTableManager
	{
		/// <summary>
		/// Load the value table from the target file
		/// </summary>
		public static bool TryLoadState(
			Path valueTableFile,
			out IValueTable result)
		{
			// Verify the requested file exists
			if (!System.IO.File.Exists(valueTableFile.ToString()))
			{
				Log.Info("Value Table file does not exist");
				result = new ValueTable();
				return false;
			}

			// Open the file to read from
			using (var fileStream = System.IO.File.OpenRead(valueTableFile.ToString()))
			using (var reader = new System.IO.BinaryReader(fileStream))
			{
				// Read the contents of the build state file
				try
				{
					result = ValueTableReader.Deserialize(reader);
					return true;
				}
				catch (Exception ex)
				{
					Log.Error($"Failed to parse value table: {ex.Message}");
					result = new ValueTable();
					return false;
				}
			}
		}

		/// <summary>
		/// Save the value table for the target file
		/// </summary>
		public static void SaveState(
			Path valueTableFile,
			IValueTable state)
		{
			var targetFolder = valueTableFile.GetParent();

			// Ensure the target directories exists
			if (!System.IO.Directory.Exists(targetFolder.ToString()))
			{
				Log.Info("Create Directory: " + targetFolder.ToString());
				System.IO.Directory.CreateDirectory(targetFolder.ToString());
			}

			// Open the file to write to
			using (var fileStream = System.IO.File.Open(valueTableFile.ToString(), System.IO.FileMode.Create, System.IO.FileAccess.Write))
			using (var writer = new System.IO.BinaryWriter(fileStream))
			{
				// Write the build state to the file stream
				ValueTableWriter.Serialize(state, writer);
			}
		}
	}
}
