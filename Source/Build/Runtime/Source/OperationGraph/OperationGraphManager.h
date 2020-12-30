// <copyright file="OperationGraphManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationGraph.h"
#include "OperationGraphReader.h"
#include "OperationGraphWriter.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The operation state manager
	/// </summary>
	export class OperationGraphManager
	{
	public:
		/// <summary>
		/// Load the operation state from the provided directory
		/// </summary>
		static bool TryLoadState(
			const Path& operationGraphFile,
			OperationGraph& result,
			FileSystemState& fileSystemState)
		{
			// Verify the requested file exists
			if (!System::IFileSystem::Current().Exists(operationGraphFile))
			{
				Log::Info("Operation graph file does not exist");
				return false;
			}

			// Open the file to read from
			auto file = System::IFileSystem::Current().OpenRead(operationGraphFile, true);

			// Read the contents of the build state file
			try
			{
				auto loadedResult = OperationGraphReader::Deserialize(file->GetInStream());
				
				// Map up the incoming file ids to the active file system state ids
				auto activeFileIdMap = std::unordered_map<FileId, FileId>();
				for (auto& fileReference : loadedResult.GetReferencedFiles())
				{
					auto activeFileId = fileSystemState.ToFileId(fileReference.second);
					auto insertResult = activeFileIdMap.emplace(fileReference.first, activeFileId);
					if (!insertResult.second)
						throw std::runtime_error("Failed to insert file id lookup");

					// Update the referenced id
					fileReference.first = activeFileId;
				}

				// Update all of the operations
				for (auto& operationReference : loadedResult.GetOperations())
				{
					auto& operation = operationReference.second;
					UpdateFileIds(operation.DeclaredInput, activeFileIdMap);
					UpdateFileIds(operation.DeclaredOutput, activeFileIdMap);
					UpdateFileIds(operation.ObservedInput, activeFileIdMap);
					UpdateFileIds(operation.ObservedOutput, activeFileIdMap);
				}

				result = std::move(loadedResult);
				return true;
			}
			catch(std::runtime_error& ex)
			{
				Log::Error(ex.what());
				return false;
			}
			catch(...)
			{
				Log::Error("Failed to parse operation graph");
				return false;
			}
		}

		/// <summary>
		/// Save the operation state for the provided directory
		/// </summary>
		static void SaveState(
			const Path& operationGraphFile,
			OperationGraph& state,
			const FileSystemState& fileSystemState)
		{
			auto targetFolder = operationGraphFile.GetParent();

			// Update the operation graph referenced files
			auto files = std::set<FileId>();
			for (auto& operationReference : state.GetOperations())
			{
				auto& operation = operationReference.second;
				files.insert(operation.DeclaredInput.begin(), operation.DeclaredInput.end());
				files.insert(operation.DeclaredOutput.begin(), operation.DeclaredOutput.end());
				files.insert(operation.ObservedInput.begin(), operation.ObservedInput.end());
				files.insert(operation.ObservedOutput.begin(), operation.ObservedOutput.end());
			}

			auto referencedFiles = std::vector<std::pair<FileId, Path>>();
			for (auto fileId : files)
			{
				referencedFiles.push_back({ fileId, fileSystemState.GetFilePath(fileId) });
			}

			state.SetReferencedFiles(std::move(referencedFiles));

			// Ensure the target directories exists
			if (!System::IFileSystem::Current().Exists(targetFolder))
			{
				Log::Info("Create Directory: " + targetFolder.ToString());
				System::IFileSystem::Current().CreateDirectory2(targetFolder);
			}

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(operationGraphFile, true);

			// Write the build state to the file stream
			OperationGraphWriter::Serialize(state, file->GetOutStream());
		}

		/// <summary>
		/// Attempt to merge the existing operation graph if it exists
		/// </summary>
		static void TryMergeExisting(
			const Path& operationGraphFile,
			OperationGraph& operationGraph,
			FileSystemState& fileSystemState)
		{
			Log::Diag("Loading previous operation graph");
			auto previousOperationGraph = OperationGraph();
			if (TryLoadState(
				operationGraphFile,
				previousOperationGraph,
				fileSystemState))
			{
				Log::Diag("Merge previous operation graph observed results");
				for (auto& activeOperationEntry : operationGraph.GetOperations())
				{
					auto& activeOperationInfo = activeOperationEntry.second;
					OperationInfo* previousOperationInfo = nullptr;
					if (previousOperationGraph.TryFindOperationInfo(activeOperationInfo.Command, previousOperationInfo))
					{
						activeOperationInfo.WasSuccessfulRun = previousOperationInfo->WasSuccessfulRun;
						activeOperationInfo.ObservedInput = previousOperationInfo->ObservedInput;
						activeOperationInfo.ObservedOutput = previousOperationInfo->ObservedOutput;
					}
				}
			}
			else
			{
				Log::Info("No valid previous build graph found");
			}
		}

	private:
		static void UpdateFileIds(std::vector<FileId>& fileIds, const std::unordered_map<FileId, FileId>& activeFileIdMap)
		{
			for (auto& fileId : fileIds)
			{
				auto findActiveFileId = activeFileIdMap.find(fileId);
				if (findActiveFileId == activeFileIdMap.end())
					throw std::runtime_error("Could not find operation file id in active map");
				fileId = findActiveFileId->second;
			}
		}
	};
}
