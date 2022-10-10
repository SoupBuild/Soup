// <copyright file="BuildEvaluateEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IEvaluateEngine.h"
#include "BuildFailedException.h"
#include "BuildHistoryChecker.h"
#include "FileSystemState.h"
#include "OperationGraph/OperationGraph.h"
#include "SystemAccessTracker.h"

namespace Soup::Core
{
	/// <summary>
	/// The core build evaluation engine that knows how to perform a build from a provided Operation Graph.
	/// </summary>
	export class BuildEvaluateEngine : public IEvaluateEngine
	{
	private:
		// Shared Runtime State
		FileSystemState& _fileSystemState;

		std::unordered_map<OperationId, int32_t> _remainingDependencyCounts;
		BuildHistoryChecker _stateChecker;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildEvaluateEngine"/> class.
		/// </summary>
		BuildEvaluateEngine(
			FileSystemState& fileSystemState) :
			_fileSystemState(fileSystemState),
			_remainingDependencyCounts(),
			_stateChecker(fileSystemState)
		{
		}

		/// <summary>
		/// Execute the entire operation graph that is referenced by this build evaluate engine
		/// </summary>
		void Evaluate(
			OperationGraph& operationGraph,
			const Path& temporaryDirectory,
			const std::vector<Path>& globalAllowedReadAccess,
			const std::vector<Path>& globalAllowedWriteAccess)
		{
			// Run all build operations in the correct order with incremental build checks
			Log::Diag("Build evaluation start");
			_remainingDependencyCounts.clear();
			CheckExecuteOperations(
				operationGraph,
				temporaryDirectory,
				globalAllowedReadAccess,
				globalAllowedWriteAccess,
				operationGraph.GetRootOperationIds());
			Log::Diag("Build evaluation end");
		}

	private:
		/// <summary>
		/// Execute the collection of build operations
		/// </summary>
		void CheckExecuteOperations(
			OperationGraph& operationGraph,
			const Path& temporaryDirectory,
			const std::vector<Path>& globalAllowedReadAccess,
			const std::vector<Path>& globalAllowedWriteAccess,
			const std::vector<OperationId>& operations)
		{
			for (auto operationId : operations)
			{
				// Check if the operation was already a child from a different path
				// Only run the operation when all of its dependencies have completed
				auto& operationInfo = operationGraph.GetOperationInfo(operationId);
				auto currentOperationSearch = _remainingDependencyCounts.find(operationId);
				int32_t remainingCount = -1;
				if (currentOperationSearch != _remainingDependencyCounts.end())
				{
					remainingCount = --currentOperationSearch->second;
				}
				else
				{
					// Get the cached total count and store the active count in the lookup
					remainingCount = operationInfo.DependencyCount - 1;
					auto insertResult = _remainingDependencyCounts.emplace(operationId, remainingCount);
					if (!insertResult.second)
						throw std::runtime_error("The operation id already existed in the remaining count lookup");
				}

				if (remainingCount == 0)
				{
					// Run the single operation
					CheckExecuteOperation(
						temporaryDirectory,
						globalAllowedReadAccess,
						globalAllowedWriteAccess,
						operationInfo);
					
					// Recursively build all of the operation children
					CheckExecuteOperations(
						operationGraph,
						temporaryDirectory,
						globalAllowedReadAccess,
						globalAllowedWriteAccess,
						operationInfo.Children);
				}
				else if (remainingCount < 0)
				{
					throw std::runtime_error("Remaining dependency count less than zero");
				}
				else
				{
					// This operation will be executed from a different path
				}
			}
		}

		/// <summary>
		/// Check if an individual operation has been run and execute if required
		/// </summary>
		void CheckExecuteOperation(
			const Path& temporaryDirectory,
			const std::vector<Path>& globalAllowedReadAccess,
			const std::vector<Path>& globalAllowedWriteAccess,
			OperationInfo& operationInfo)
		{
			// Check if each source file is out of date and requires a rebuild
			Log::Diag("Check for previous operation invocation");

			// Check if this operation was run before
			auto buildRequired = false;
			if (operationInfo.WasSuccessfulRun)
			{
				// Check if the executable has changed since the last run
				bool executableOutOfDate = false;
				if (operationInfo.Command.Executable != Path("writefile.exe"))
				{
					// Only check for "real" executables
					auto executableFileId = _fileSystemState.ToFileId(
						operationInfo.Command.Executable,
						operationInfo.Command.WorkingDirectory);
					if (_stateChecker.IsOutdated(operationInfo.EvaluateTime, executableFileId))
					{
						executableOutOfDate = true;
					}
				}

				// Perform the incremental build checks
				if (executableOutOfDate ||
					_stateChecker.IsOutdated(operationInfo.ObservedOutput, operationInfo.ObservedInput))
				{
					buildRequired = true;
				}
				else
				{
					Log::Info("Up to date");
				}
			}
			else
			{
				// The build command has not been run before
				Log::Info("Operation has no successful previous invocation");
				buildRequired = true;
			}

			if (buildRequired)
			{
				Log::HighPriority(operationInfo.Title);
				auto messageBuilder = std::stringstream();
				messageBuilder << "Execute: [" << operationInfo.Command.WorkingDirectory.ToString() << "] ";
				messageBuilder << operationInfo.Command.Executable.ToString();
				messageBuilder << " " << operationInfo.Command.Arguments;
				Log::Diag(messageBuilder.str());

				// Check for special in-process write operations
				if (operationInfo.Command.Executable == Path("writefile.exe"))
				{
					ExecuteWriteFileOperation(operationInfo);
				}
				else
				{
					ExecuteOperation(
						temporaryDirectory,
						globalAllowedReadAccess,
						globalAllowedWriteAccess,
						operationInfo);
				}
			}
			else
			{
				Log::Info(operationInfo.Title);
			}
		}

		/// <summary>
		/// Execute a single build operation
		/// </summary>
		void ExecuteWriteFileOperation(
			OperationInfo& operationInfo)
		{
			Log::Info("Execute InProcess WriteFile");

			// Pull out the file path argument
			auto findSecondQuote = operationInfo.Command.Arguments.find('\"', 1);
			if (findSecondQuote == std::string::npos)
			{
				Log::Error("WriteFile path argument malformed");
				throw BuildFailedException();
			}

			auto fileName = Path(std::string_view(operationInfo.Command.Arguments).substr(1, findSecondQuote - 1));
			Log::Info("WritFile: " + fileName.ToString());

			auto filePath = fileName.HasRoot() ? fileName : operationInfo.Command.WorkingDirectory + fileName;
			auto content = std::string_view(operationInfo.Command.Arguments).substr(
				findSecondQuote + 3,
				operationInfo.Command.Arguments.size() - findSecondQuote - 4);

			// Open the file to write to
			auto file = System::IFileSystem::Current().OpenWrite(filePath, false);
			file->GetOutStream() << content;

			operationInfo.ObservedInput = {};
			operationInfo.ObservedOutput = {
				_fileSystemState.ToFileId(filePath, operationInfo.Command.WorkingDirectory),
			};

			// Mark this operation as successful to enable future incremental builds
			operationInfo.WasSuccessfulRun = true;
			operationInfo.EvaluateTime = std::chrono::system_clock::now();

			// Ensure the File System State is notified of any output files that have changed
			_fileSystemState.CheckFileWriteTimes(operationInfo.ObservedOutput);
		}

		/// <summary>
		/// Execute a single build operation
		/// </summary>
		void ExecuteOperation(
			const Path& temporaryDirectory,
			const std::vector<Path>& globalAllowedReadAccess,
			const std::vector<Path>& globalAllowedWriteAccess,
			OperationInfo& operationInfo)
		{
			auto callback = std::make_shared<SystemAccessTracker>();

			// Replace callback with logger and existing
			// auto callbackWrapper = std::make_shared<Monitor::ForkCallbackLogger>(
			// 	callback,
			// 	std::make_shared<Monitor::DetourCallbackLogger>(std::cout));

			// Add the temp folder to the environment
			auto environment = std::map<std::string, std::string>();
			environment.emplace("TEMP", temporaryDirectory.ToString());
			environment.emplace("TMP", temporaryDirectory.ToString());

			// Allow access to the declared inputs/outputs
			auto allowedReadAccess = _fileSystemState.GetFilePaths(operationInfo.ReadAccess);
			auto allowedWriteAccess = _fileSystemState.GetFilePaths(operationInfo.WriteAccess);

			// Allow access to the global overrides
			std::copy(globalAllowedReadAccess.begin(), globalAllowedReadAccess.end(), std::back_inserter(allowedReadAccess));
			std::copy(globalAllowedWriteAccess.begin(), globalAllowedWriteAccess.end(), std::back_inserter(allowedWriteAccess));

			Log::Diag("Allowed Read Access:");
			for (auto& file : allowedReadAccess)
				Log::Diag(file.ToString());
			Log::Diag("Allowed Write Access:");
			for (auto& file : allowedWriteAccess)
				Log::Diag(file.ToString());

			bool enableAccessChecks = true;
			auto process = Monitor::IDetourProcessManager::Current().CreateDetourProcess(
				operationInfo.Command.Executable,
				operationInfo.Command.Arguments,
				operationInfo.Command.WorkingDirectory,
				environment,
				callback, // callbackWrapper,
				enableAccessChecks,
				allowedReadAccess,
				allowedWriteAccess);

			process->Start();
			process->WaitForExit();

			auto stdOut = process->GetStandardOutput();
			auto stdErr = process->GetStandardError();
			auto exitCode = process->GetExitCode();

			// Check the result of the monitor
			callback->VerifyResult();

			if (!stdOut.empty())
			{
				// Upgrade output to a warning if the command fails
				if (exitCode != 0)
					Log::Warning(stdOut);
				else
					Log::Info(stdOut);
			}

			// If there was any error output then the build failed
			// TODO: Find warnings + errors
			if (!stdErr.empty())
			{
				Log::Error(stdErr);
			}

			if (exitCode == 0)
			{
				// Save off the build graph for future builds
				auto input = std::vector<Path>();
				for (auto& value : callback->GetInput())
				{
					auto path = Path(value);
					// Log::Diag("ObservedInput: " + path.ToString());
					input.push_back(std::move(path));
				}

				auto output = std::vector<Path>();
				for (auto& value : callback->GetOutput())
				{
					auto path = Path(value);
					// Log::Diag("ObservedOutput: " + path.ToString());
					output.push_back(std::move(path));
				}

				operationInfo.ObservedInput = _fileSystemState.ToFileIds(input, operationInfo.Command.WorkingDirectory);
				operationInfo.ObservedOutput = _fileSystemState.ToFileIds(output, operationInfo.Command.WorkingDirectory);

				// Mark this operation as successful to enable future incremental builds
				operationInfo.WasSuccessfulRun = true;
				operationInfo.EvaluateTime = std::chrono::system_clock::now();

				// Ensure the File System State is notified of any output files that have changed
				_fileSystemState.CheckFileWriteTimes(operationInfo.ObservedOutput);
			}
			else
			{
				// Leave the previous state untouched and abandon the remaining operations
				Log::Error("Operation exited with non-success code: " + std::to_string(exitCode));
				throw BuildFailedException();
			}
		}
	};
}
