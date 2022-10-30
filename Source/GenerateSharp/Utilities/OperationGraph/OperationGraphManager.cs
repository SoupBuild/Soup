﻿// <copyright file="OperationGraphManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Runtime;
using System.Collections.Generic;
using System.Text;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The operation state manager
	/// </summary>
	public static class OperationGraphManager
	{
		/// <summary>
		/// Load the operation state from the provided directory
		/// </summary>
		public static bool TryLoadState(
			Path operationGraphFile,
			FileSystemState fileSystemState,
			out OperationGraph result)
		{
			// Verify the requested file exists
			if (!LifetimeManager.Get<IFileSystem>().Exists(operationGraphFile))
			{
				Log.Info("Operation graph file does not exist");
				result = new OperationGraph();
				return false;
			}

			// Open the file to read from
			using var file = LifetimeManager.Get<IFileSystem>().OpenRead(operationGraphFile);
			using var reader = new System.IO.BinaryReader(file.GetInStream(), Encoding.UTF8, true);

			// Read the contents of the build state file
			try
			{
				var loadedResult = OperationGraphReader.Deserialize(reader);

				// Map up the incoming file ids to the active file system state ids
				var activeFileIdMap = new Dictionary<FileId, FileId>();
				for (var i = 0; i < loadedResult.GetReferencedFiles().Count; i++)
				{
					var fileReference = loadedResult.GetReferencedFiles()[i];
					var activeFileId = fileSystemState.ToFileId(fileReference.Path);
					activeFileIdMap.Add(fileReference.FileId, activeFileId);

					// Update the referenced id
					fileReference.FileId = activeFileId;
				}

				// Update all of the operations
				foreach (var operationReference in loadedResult.GetOperations())
				{
					var operation = operationReference.Value;
					UpdateFileIds(operation.DeclaredInput, activeFileIdMap);
					UpdateFileIds(operation.DeclaredOutput, activeFileIdMap);
				}

				result = loadedResult;
				return true;
			}
			catch
			{
				Log.Error("Failed to parse operation graph");
				result = new OperationGraph();
				return false;
			}
		}

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
