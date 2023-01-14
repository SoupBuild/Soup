// <copyright file="OperationGraphGenerator.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Core::Generate
{
	/// <summary>
	/// The cached operation graph that is used to track input/output mappings for previous build
	/// executions to support incremental builds
	/// </summary>
	class OperationGraphGenerator
	{
	private:
		FileSystemState& _fileSystemState;
		std::vector<Path> _readAccessList;
		std::vector<Path> _writeAccessList;
		OperationId _uniqueId;
		OperationGraph _graph;

		// Running state used to build graph dynamically
		std::map<FileId, std::vector<OperationId>> _inputFileLookup;
		std::map<FileId, OperationId> _outputFileLookup;
		std::map<FileId, OperationId> _outputDirectoryLookup;

	public:
		OperationGraphGenerator(
			FileSystemState& fileSystemState,
			std::vector<Path> readAccessList,
			std::vector<Path> writeAccessList) :
			_fileSystemState(fileSystemState),
			_readAccessList(std::move(readAccessList)),
			_writeAccessList(std::move(writeAccessList)),
			_uniqueId(0),
			_graph(),
			_inputFileLookup(),
			_outputFileLookup(),
			_outputDirectoryLookup()
		{
		}

		/// <summary>
		/// Create an operation graph from the provided generated graph
		/// At the same time verify that there are no cycles
		/// </summary>
		void CreateOperation(
			std::string title,
			Path executable,
			std::string arguments,
			Path workingDirectory,
			std::vector<Path> declaredInput,
			std::vector<Path> declaredOutput)
		{
			Log::Diag("Create Operation: " + title);

			if (!workingDirectory.HasRoot())
				throw std::runtime_error("Working directory must be an absolute path.");

			// Build up the operation unique command
			auto commandInfo = CommandInfo(
				std::move(workingDirectory),
				std::move(executable),
				std::move(arguments));

			// Ensure this is the a unique operation
			if (_graph.HasCommand(commandInfo))
			{
				throw std::runtime_error("Operation with this command already exists.");
			}

			// Verify allowed read access
			auto readAccess = std::vector<Path>();
			if (!IsAllowedAccess(_readAccessList, declaredInput, commandInfo.WorkingDirectory, readAccess))
			{
				throw std::runtime_error("Operation does not have permission to read requested input.");
			}

			Log::Diag("Read Access Subset:");
			for (auto& file : readAccess)
				Log::Diag(file.ToString());

			// Verify allowed write access
			auto writeAccess = std::vector<Path>();
			if (!IsAllowedAccess(_writeAccessList, declaredOutput, commandInfo.WorkingDirectory, writeAccess))
			{
				throw std::runtime_error("Operation does not have permission to write requested output.");
			}

			Log::Diag("Write Access Subset:");
			for (auto& file : writeAccess)
				Log::Diag(file.ToString());

			// Generate a unique id for this new operation
			_uniqueId++;
			auto operationId = _uniqueId;

			// Resolve the requested files to unique ids
			auto declaredInputFileIds = _fileSystemState.ToFileIds(declaredInput, commandInfo.WorkingDirectory);
			auto declaredOutputFileIds = _fileSystemState.ToFileIds(declaredOutput, commandInfo.WorkingDirectory);
			auto readAccessFileIds = _fileSystemState.ToFileIds(readAccess, commandInfo.WorkingDirectory);
			auto writeAccessFileIds = _fileSystemState.ToFileIds(writeAccess, commandInfo.WorkingDirectory);

			// Build up the declared build operation
			auto operationInfo = OperationInfo(
				operationId,
				title,
				commandInfo,
				declaredInputFileIds,
				declaredOutputFileIds,
				readAccessFileIds,
				writeAccessFileIds);
			auto& operationInfoReference = _graph.AddOperation(std::move(operationInfo));

			StoreLookupInfo(operationInfoReference);
			ResolveDependencies(operationInfoReference);
		}

		OperationGraph FinalizeGraph()
		{
			// Add any operation with zero dependencies to the root
			auto rootOperations = std::vector<OperationId>();
			for (auto& [_, activeOperationInfo] : _graph.GetOperations())
			{
				if (activeOperationInfo.DependencyCount == 0)
				{
					activeOperationInfo.DependencyCount = 1;
					rootOperations.push_back(activeOperationInfo.Id);
				}
			}

			_graph.SetRootOperationIds(std::move(rootOperations));

			// Remove extra dependency references that are already covered by upstream references
			auto recursiveChildren = std::map<OperationId, std::set<OperationId>>();
			BuildRecursiveChildSets(recursiveChildren, _graph.GetRootOperationIds());
			for (auto& [_, operation] : _graph.GetOperations())
			{
				// Check each child to see if it is already covered by another child
				for (auto iterator = operation.Children.begin(); iterator != operation.Children.end();)
				{
					auto childId = *iterator;
					auto isDuplicate = std::any_of(
						operation.Children.cbegin(),
						operation.Children.cend(),
						[&](OperationId secondChildId)
						{
							if (secondChildId != childId)
							{
								auto childRecursiveSet = recursiveChildren[secondChildId];
								return childRecursiveSet.contains(childId);
							}
							else
							{
								return false;
							}
						});

					if (isDuplicate)
					{
						// Update the child dependency count
						auto childOperation = _graph.GetOperationInfo(childId);
						childOperation.DependencyCount--;

						// Remove the duplicate
						iterator = operation.Children.erase(iterator);
					}
					else
					{
						iterator++;
					}
				}
			}

			return _graph;
		}

	private:
		void StoreLookupInfo(const OperationInfo& operationInfo)
		{
			// Store the operation in the required file lookups to ensure single target
			// and help build up the dependency graph
			for (auto file : operationInfo.DeclaredOutput)
			{
				auto& filePath = _fileSystemState.GetFilePath(file);
				if (filePath.HasFileName())
				{
					CheckSetOutputFileOperation(file, operationInfo);
				}
				else
				{
					CheckSetOutputDirectoryOperation(file, operationInfo);
				}
			}

			for (auto file : operationInfo.DeclaredInput)
			{
				auto& filePath = _fileSystemState.GetFilePath(file);
				if (filePath.HasFileName())
				{
					AddInputFileOperation(file, operationInfo);
				}
			}
		}

		void ResolveDependencies(OperationInfo& operationInfo)
		{
			// Build up the child dependencies based on the operations that use this operations output files

			// Check for inputs that match previous output files
			for (auto file : operationInfo.DeclaredInput)
			{
				OperationId matchedOperation;
				if (TryGetOutputFileOperation(file, matchedOperation))
				{
					// The active operation must run after the matched output operation
					CheckAddChildOperation(_graph.GetOperationInfo(matchedOperation), operationInfo);
				}
			}

			// Check for outputs that match previous input files
			for (auto file : operationInfo.DeclaredOutput)
			{
				std::vector<OperationId> matchedOperations;
				if (TryGetInputFileOperations(file, matchedOperations))
				{
					for (auto matchedOperation : matchedOperations)
					{
						// The active operation must run before the matched output operation
						CheckAddChildOperation(operationInfo, _graph.GetOperationInfo(matchedOperation));
					}
				}
			}

			// Check for output files that are under previous output directories
			for (auto file : operationInfo.DeclaredOutput)
			{
				auto& filePath = _fileSystemState.GetFilePath(file);
				auto parentDirectory = filePath.GetParent();
				auto done = false;
				while (!done)
				{
					FileId parentDirectoryId;
					if (_fileSystemState.TryFindFileId(parentDirectory, parentDirectoryId))
					{
						OperationId matchedOperation;
						if (TryGetOutputDirectoryOperation(parentDirectoryId, matchedOperation))
						{
							// The matched directory output operation must run before the active operation
							CheckAddChildOperation(_graph.GetOperationInfo(matchedOperation), operationInfo);
						}
					}

					// Get the next parent directory
					auto nextParentDirectory = parentDirectory.GetParent();
					done = nextParentDirectory.ToString().size() == parentDirectory.ToString().size();
					parentDirectory = nextParentDirectory;
				}
			}

			// Ensure there are no circular references
			auto closure = std::set<OperationId>();
			BuildChildClosure(operationInfo.Children, closure);
			if (closure.contains(operationInfo.Id))
			{
				throw std::runtime_error("Operation introduced circular reference");
			}
		}

		void BuildChildClosure(
			const std::vector<OperationId>& operations,
			std::set<OperationId>& closure)
		{
			for (auto operationId : operations)
			{
				// Check if this node was already handled in a different branch
				if (!closure.contains(operationId))
				{
					closure.insert(operationId);

					auto& operation = _graph.GetOperationInfo(operationId);
					BuildChildClosure(operation.Children, closure);
				}
			}
		}

		void BuildRecursiveChildSets(
			std::map<OperationId, std::set<OperationId>>& recursiveChildren,
			const std::vector<OperationId>& operations)
		{
			for (auto operationId : operations)
			{
				auto& operation = _graph.GetOperationInfo(operationId);
				BuildRecursiveChildSets(recursiveChildren, operation.Children);

				// Check if this node was already handled in a different branch
				if (!recursiveChildren.contains(operationId))
				{
					auto childSet = std::set<OperationId>();
					std::copy(operation.Children.begin(), operation.Children.end(), std::inserter(childSet, childSet.end()));
					for (auto childId : operation.Children)
					{
						auto findChildRecursiveSet = recursiveChildren.find(childId);
						if (findChildRecursiveSet == recursiveChildren.end())
							throw std::runtime_error("Recursive child was missing");
						auto& childRecursiveSet = findChildRecursiveSet->second;
						std::copy(childRecursiveSet.begin(), childRecursiveSet.end(), std::inserter(childSet, childSet.end()));
					}

					recursiveChildren.emplace(operationId, std::move(childSet));
				}
			}
		}

		bool IsAllowedAccess(
			const std::vector<Path>& accessList,
			const std::vector<Path>& files,
			const Path& workingDirectory,
			std::vector<Path>& usedAccessList)
		{
			usedAccessList.clear();

			auto accessSet = std::set<Path>();
			for (auto& file : files)
			{
				auto accessDirectory = GetAllowedAccess(accessList, file, workingDirectory);
				if (!accessDirectory.has_value())
				{
					// The file was not under any of the provided directories
					Log::Error("File access denied: " + file.ToString());
					return false;
				}
				else
				{
					accessSet.insert(std::move(accessDirectory.value()));
				}
			}

			std::copy(accessSet.begin(), accessSet.end(), std::back_inserter(usedAccessList));
			return true;
		}

		std::optional<Path> GetAllowedAccess(
			const std::vector<Path>& accessList,
			const Path& file,
			const Path& workingDirectory)
		{
			// Combine the working directory if a relative file path
			auto absoluteFile = file;
			if (!absoluteFile.HasRoot())
				absoluteFile = workingDirectory + absoluteFile;

			auto& fileString = absoluteFile.ToString();
			for (auto& accessDirectory : accessList)
			{
				if (fileString.starts_with(accessDirectory.ToString()))
				{
					return accessDirectory;
				}
			}

			return std::nullopt;
		}

		void CheckAddChildOperation(OperationInfo& parentOperation, OperationInfo& childOperation)
		{
			// Check if this item is already known
			auto findResult = std::find(
				parentOperation.Children.begin(),
				parentOperation.Children.end(),
				childOperation.Id);
			if (findResult == parentOperation.Children.end())
			{
				// Keep track of the parent child references
				parentOperation.Children.push_back(childOperation.Id);
				childOperation.DependencyCount++;
			}
		}

		bool TryGetInputFileOperations(
			FileId file,
			std::vector<OperationId>& operations)
		{
			auto findResult = _inputFileLookup.find(file);
			if (findResult != _inputFileLookup.end())
			{
				operations = findResult->second;
				return true;
			}
			else
			{
				operations.clear();
				return false;
			}
		}

		bool TryGetOutputFileOperation(
			FileId file,
			OperationId& operationId)
		{
			auto findResult = _outputFileLookup.find(file);
			if (findResult != _outputFileLookup.end())
			{
				operationId = findResult->second;
				return true;
			}
			else
			{
				operationId = 0;
				return false;
			}
		}

		bool TryGetOutputDirectoryOperation(
			FileId file,
			OperationId& operationId)
		{
			auto findResult = _outputDirectoryLookup.find(file);
			if (findResult != _outputDirectoryLookup.end())
			{
				operationId = findResult->second;
				return true;
			}
			else
			{
				operationId = 0;
				return false;
			}
		}

		void CheckSetOutputFileOperation(
			FileId file,
			const OperationInfo& operation)
		{
			auto findResult = _outputFileLookup.find(file);
			if (findResult != _outputFileLookup.end())
			{
				auto& filePath = _fileSystemState.GetFilePath(file);
				auto& existingOperation = _graph.GetOperationInfo(findResult->second);
				throw std::runtime_error("File \"" +  filePath.ToString() + "\" already written to by operation \"" + existingOperation.Title + "\"");
			}
			else
			{
				_outputFileLookup.emplace(file, operation.Id);
			}
		}

		void CheckSetOutputDirectoryOperation(
			FileId file,
			const OperationInfo& operation)
		{
			auto findResult = _outputDirectoryLookup.find(file);
			if (findResult != _outputDirectoryLookup.end())
			{
				auto& filePath = _fileSystemState.GetFilePath(file);
				auto& existingOperation = _graph.GetOperationInfo(findResult->second);
				throw std::runtime_error("Directory \"" + filePath.ToString() + "\" already written to by operation \"" + existingOperation.Title + "\"");
			}
			else
			{
				_outputDirectoryLookup.emplace(file, operation.Id);
			}
		}

		void AddInputFileOperation(
			FileId file,
			const OperationInfo& operation)
		{
			auto findResult = _inputFileLookup.find(file);
			if (findResult != _inputFileLookup.end())
			{
				auto& operations = findResult->second;
				operations.push_back(operation.Id);
			}
			else
			{
				_inputFileLookup.emplace(
					file,
					std::vector<OperationId>(
					{
						operation.Id,
					}));
			}
		}
	};
}
