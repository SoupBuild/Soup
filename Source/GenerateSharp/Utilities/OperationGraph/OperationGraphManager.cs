// <copyright file="OperationGraphManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Runtime;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The operation state manager
	/// </summary>
	public static class OperationGraphManager
	{
		/// <summary>
		/// Save the operation state for the provided directory
		/// </summary>
		public static void SaveState(
			Path operationGraphFile,
			OperationGraph state,
			FileSystemState fileSystemState)
		{
			var targetFolder = operationGraphFile.GetParent();

			// Update the operation graph referenced files
			var files = new HashSet<FileId>();
			foreach (var operationReference in state.GetOperations())
			{
				var operation = operationReference.Value;
				files.UnionWith(operation.DeclaredInput);
				files.UnionWith(operation.DeclaredOutput);
				files.UnionWith(operation.ReadAccess);
				files.UnionWith(operation.WriteAccess);
			}

			var referencedFiles = new List<(FileId FileId, Path Path)>();
			foreach (var fileId in files)
			{
				referencedFiles.Add((fileId, fileSystemState.GetFilePath(fileId)));
			}

			state.SetReferencedFiles(referencedFiles);

			// Ensure the target directories exists
			if (!LifetimeManager.Get<IFileSystem>().Exists(targetFolder))
			{
				Log.Info("Create Directory: " + targetFolder.ToString());
				LifetimeManager.Get<IFileSystem>().CreateDirectory2(targetFolder);
			}

			// Open the file to write to
			using var fileStream = System.IO.File.Open(
				operationGraphFile.ToString(),
				System.IO.FileMode.Create,
				System.IO.FileAccess.Write);
			using var writer = new System.IO.BinaryWriter(fileStream);

			// Write the build state to the file stream
			OperationGraphWriter.Serialize(state, writer);
		}

		private static void UpdateFileIds(IList<FileId> fileIds, IDictionary<FileId, FileId> activeFileIdMap)
		{
			for (var i = 0; i < fileIds.Count; i++)
			{
				var findActiveFileId = activeFileIdMap[fileIds[i]];
				fileIds[i] = findActiveFileId;
			}
		}
	}
}
