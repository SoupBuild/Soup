// <copyright file="ValueTableManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Utilities;
using System.Collections.Generic;

namespace Soup.Build.Generate
{
	internal class ValueTable : Dictionary<string, Value>, IValueTable
	{
	}

	internal class ValueList : List<Value>, IValueList
	{
	}

	/// <summary>
	/// The Value Table state manager
	/// </summary>
	internal static class ValueTableManager
	{
		/// <summary>
		/// Load the value table from the target file
		/// </summary>
		public static bool TryLoadState(
			string valueTableFile,
			IValueTable result)
		{
			// Verify the requested file exists
			if (!System.IO.File.Exists(valueTableFile))
			{
				Log.Info("Value Table file does not exist");
				return false;
			}

			// Open the file to read from
			var file = System.IO.File.Open(valueTableFile, System.IO.FileMode.Open, System.IO.FileAccess.Read);

			// Read the contents of the build state file
			try
			{
				result = ValueTableReader.Deserialize(file->GetInStream());
				return true;
			}
			catch
			{
				Log.Error("Failed to parse value table");
				return false;
			}
		}

		/// <summary>
		/// Save the value table for the target file
		/// </summary>
		static void SaveState(
			Path valueTableFile,
			ValueTable state)
		{
			var targetFolder = valueTableFile.GetParent();

			// Ensure the target directories exists
			if (!System.IO.Directory.Exists(targetFolder))
			{
				Log.Info("Create Directory: " + targetFolder.ToString());
				System.IO.Directory.CreateDirectory(targetFolder);
			}

			// Open the file to write to
			var file = System.IO.File.Open(valueTableFile, System.IO.FileMode.OpenOrCreate, System.IO.FileAccess.Write);

			// Write the build state to the file stream
			ValueTableWriter.Serialize(state, file->GetOutStream());
		}
	}
}
