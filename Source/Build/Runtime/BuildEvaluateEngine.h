// <copyright file="BuildEvaluateEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildFailedException.h"
#include "BuildHistoryChecker.h"
#include "FileSystemState/FileSystemState.h"
#include "OperationGraph/OperationGraph.h"
#include "SystemAccessTracker.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The core build evaluation engine that knows how to perform a build from a provided Operation Graph.
	/// </summary>
	export class BuildEvaluateEngine
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildEvaluateEngine"/> class.
		/// </summary>
		BuildEvaluateEngine(
			FileSystemState& fileSystemState,
			OperationGraph& operationGraph) :
			_fileSystemState(fileSystemState),
			_operationGraph(operationGraph),
			_remainingDependencyCounts(),
			_stateChecker(fileSystemState)
		{
		}

		/// <summary>
		/// Execute the entire operation graph that is referenced by this build evaluate engine
		/// </summary>
		void Evaluate()
		{
			// Run all build operations in the correct order with incremental build checks
			Log::Diag("Build evaluation start");
			CheckExecuteOperations(_operationGraph.GetRootOperationIds());
			Log::Diag("Build evaluation end");
		}

	private:
		/// <summary>
		/// Execute the collection of build operations
		/// </summary>
		void CheckExecuteOperations(
			const std::vector<OperationId>& operations)
		{
			for (auto operationId : operations)
			{
				// Check if the operation was already a child from a different path
				// Only run the operation when all of its dependencies have completed
				auto& operationInfo = _operationGraph.GetOperationInfo(operationId);
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
					ExecuteOperation(operationInfo);
					
					// Recursively build all of the operation children
					CheckExecuteOperations(operationInfo.Children);
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
		/// Execute a single build operation
		/// </summary>
		void ExecuteOperation(
			OperationInfo& operationInfo)
		{
			// Check if each source file is out of date and requires a rebuild
			Log::Diag("Check for previous operation invocation");

			// Check if this operation was run before
			auto buildRequired = false;
			if (operationInfo.WasSuccessfulRun)
			{
				// Perform the incremental build checks
				if (_stateChecker.IsOutdated(
					operationInfo.ObservedOutput,
					operationInfo.ObservedInput))
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

				auto callback = std::make_shared<SystemAccessTracker>();
				auto process = Monitor::IDetourProcessManager::Current().CreateDetourProcess(
					operationInfo.Command.Executable,
					operationInfo.Command.Arguments,
					operationInfo.Command.WorkingDirectory,
					callback);

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
						input.push_back(Path(value));

					auto output = std::vector<Path>();
					for (auto& value : callback->GetOutput())
						output.push_back(Path(value));

					operationInfo.ObservedInput = _fileSystemState.ToFileIds(input, operationInfo.Command.WorkingDirectory);
					operationInfo.ObservedOutput = _fileSystemState.ToFileIds(output, operationInfo.Command.WorkingDirectory);

					// Mark this operation as successful to enable future incremental builds
					operationInfo.WasSuccessfulRun = true;

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
			else
			{
				Log::Info(operationInfo.Title);
			}
		}

	private:
		FileSystemState& _fileSystemState;
		OperationGraph& _operationGraph;
		std::unordered_map<OperationId, int32_t> _remainingDependencyCounts;
		BuildHistoryChecker _stateChecker;
	};
}
