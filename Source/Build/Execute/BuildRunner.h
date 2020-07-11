// <copyright file="BuildRunner.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildHistory.h"
#include "SystemAccessTracker.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The build runner
	/// </summary>
	export class BuildRunner
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildRunner"/> class.
		/// </summary>
		BuildRunner(Path workingDirectory) :
			_workingDirectory(std::move(workingDirectory)),
			_dependencyCounts(),
			_buildHistory(),
			_stateChecker()
		{
		}

		// TODO: Convert vector to const when we have a const version of the operation wrapper.
		void Execute(
			Utilities::BuildOperationListWrapper& operations,
			const Path& objectDirectory,
			bool forceBuild)
		{
			// Load the previous build state if performing an incremental build
			auto targetDirectory = _workingDirectory + objectDirectory;
			if (!forceBuild)
			{
				Log::Diag("Loading previous build state");
				if (!BuildHistoryManager::TryLoadState(targetDirectory, _buildHistory))
				{
					Log::Info("No previous state found, full rebuild required");
					_buildHistory = BuildHistory();
					forceBuild = true;
				}
			}

			// Build the initial operation dependency set to
			// ensure operations are built in the correct order 
			// and that there are no cycles
			auto emptyParentSet = std::set<uint64_t>();
			BuildDependencies(operations, emptyParentSet);

			// Run all build operations in the correct order with incremental build checks
			CheckExecuteOperations(operations, forceBuild);

			Log::Info("Saving updated build state");
			BuildHistoryManager::SaveState(targetDirectory, _buildHistory);

			Log::HighPriority("Done");
		}

	private:
		/// <summary>
		/// Build dependencies
		/// </summary>
		void BuildDependencies(
			Utilities::BuildOperationListWrapper& operations,
			const std::set<uint64_t>& parentSet)
		{
			for (auto i = 0; i < operations.GetSize(); i++)
			{
				// Make sure there are no cycles using the address as a unique id
				auto operation = operations.GetValueAt(i);
				auto operationId = reinterpret_cast<uint64_t>(operation.GetRaw());
				if (parentSet.contains(operationId))
					throw std::runtime_error("A build operation graph must be acyclic.");
				
				// Check if the operation was already a child from a different path
				auto currentOperationSearch = _dependencyCounts.find(operationId);
				if (currentOperationSearch != _dependencyCounts.end())
				{
					// Increment the dependency count
					currentOperationSearch->second++;
				}
				else
				{
					// Insert a new entry with a single count
					_dependencyCounts.emplace(operationId, 1);
					
					// Recurse to children only the first time we see an operation
					auto updatedParentSet = parentSet;
					updatedParentSet.insert(operationId);
					BuildDependencies(operation.GetChildList(), updatedParentSet);
				}
			}
		}

		/// <summary>
		/// Execute the collection of build operations
		/// </summary>
		void CheckExecuteOperations(
			Utilities::BuildOperationListWrapper& operations,
			bool forceBuild)
		{
			for (auto i = 0; i < operations.GetSize(); i++)
			{
				// Check if the operation was already a child from a different path
				// Make sure there are no cycles using the address as a unique id
				auto operation = operations.GetValueAt(i);
				auto operationId = reinterpret_cast<uint64_t>(operation.GetRaw());
				auto currentOperationSearch = _dependencyCounts.find(operationId);
				if (currentOperationSearch != _dependencyCounts.end())
				{
					auto remainingCount = --currentOperationSearch->second;
					if (remainingCount == 0)
					{
						ExecuteOperation(operation, forceBuild);
					}
					else
					{
						// This operation will be executed from a different path
					}
				}
				else
				{
					throw std::runtime_error("A operation id was missing from the dependency collection.");
				}
			}
		}

		/// <summary>
		/// Execute a single build operation and all of its children
		/// </summary>
		void ExecuteOperation(
			Utilities::BuildOperationWrapper& operation,
			bool forceBuild)
		{
			// Build up the operation unique command
			std::stringstream commandBuilder;
			commandBuilder << operation.GetWorkingDirectory() << " : " << operation.GetExecutable() << " " << operation.GetArguments();
			auto command = commandBuilder.str();

			bool buildRequired = forceBuild;
			if (!forceBuild)
			{
				// Check if each source file is out of date and requires a rebuild
				Log::Diag("Check for updated source");
				
				// Check if this operation is in the build history
				const OperationInfo* operationInfo;
				if (_buildHistory.TryFindOperationInfo(command, operationInfo))
				{
					// Check if any of the input files have changed since last build
					if (_stateChecker.IsOutdated(
						operationInfo->Output,
						operationInfo->Input,
						Path(operation.GetWorkingDirectory())))
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
					buildRequired = true;
				}
			}

			if (buildRequired)
			{
				Log::HighPriority(operation.GetTitle());
				auto executable = Path(operation.GetExecutable());
				auto arguments = std::string(operation.GetArguments());
				auto workingDirectory = Path(operation.GetWorkingDirectory());
				auto message = "Execute: " + executable.ToString() + " " + arguments;
				Log::Diag(message);

				// auto process = System::IProcessManager::Current().CreateProcess(
				// 	executable,
				// 	arguments,
				// 	workingDirectory);

				auto callback = std::make_shared<SystemAccessTracker>(std::cout);
				auto process = Monitor::IDetourProcessManager::Current().CreateDetourProcess(
					executable,
					arguments,
					workingDirectory,
					callback);

				process->Start();
				process->WaitForExit();

				auto stdOut = process->GetStandardOutput();
				auto stdErr = process->GetStandardError();
				auto exitCode = process->GetExitCode();

				// Save off the build history for future builds
				auto operationInfo = OperationInfo(
					command,
					operation.GetInputFileList().CopyAsPathVector(),
					operation.GetOutputFileList().CopyAsPathVector());

				// Try parse includes if available
				auto cleanOutput = std::stringstream();
				auto headerIncludes = std::vector<Path>();
				if (TryParsesHeaderIncludes(
					operation,
					stdOut,
					headerIncludes,
					cleanOutput))
				{
					operationInfo.Input.insert(
						operationInfo.Input.end(),
						headerIncludes.begin(),
						headerIncludes.end());

					// Replace the output string with the clean version
					stdOut = cleanOutput.str();
				}

				_buildHistory.AddOperationInfo(std::move(operationInfo));

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

				if (exitCode != 0)
				{
					throw std::runtime_error("Compiler Object Error: " + std::to_string(exitCode));
				}
			}
			else
			{
				Log::Info(operation.GetTitle());
			}

			// Recursively build all of the operation children
			// Note: Force build if this operation was built
			CheckExecuteOperations(operation.GetChildList(), buildRequired);
		}

		bool TryParsesHeaderIncludes(
			Utilities::BuildOperationWrapper& operation,
			const std::string& output,
			std::vector<Path>& headerIncludes,
			std::stringstream& cleanOutput)
		{
			// Check for any cpp input files
			auto executable = Path(operation.GetExecutable());
			for (auto& inputFile : operation.GetInputFileList().CopyAsPathVector())
			{
				if (inputFile.GetFileExtension() == ".cpp")
				{
					// Parse known compiler output
					if (executable.GetFileName() == "cl.exe")
					{
						headerIncludes = ParseMSVCIncludes(inputFile, output, cleanOutput);
						return true;
					}
					else if (executable.GetFileName() == "clang++.exe")
					{
						headerIncludes = ParseMSVCIncludes(inputFile, output, cleanOutput);
						return true;
					}
				}
			}

			headerIncludes = std::vector<Path>();
			return false;
		}

		std::vector<Path> ParseClangIncludes(
			const Path& file,
			const std::string& output,
			std::stringstream& cleanOutput)
		{
			// Add the root file
			std::set<std::string> input;
			input.insert(file.ToString());

			std::stringstream content(output);
			std::string line;
			while (std::getline(content, line))
			{
				// TODO: Getline is dumb and uses newline on windows
				if (line[line.size() - 1] == '\r')
				{
					line.resize(line.size() - 1);
				}

				auto includeDepth = GetClangIncludeDepth(line);
				if (includeDepth > 0)
				{
					// Parse the file reference
					auto includeFile = Path(line.substr(includeDepth + 1));
					input.insert(includeFile.ToString());
				}
				else
				{
					// Not an include, pass along
					cleanOutput << line << "\n";
				}
			}

			auto result = std::vector<Path>();
			for (auto& value : input)
			{
				result.push_back(Path(value));
			}

			return result;
		}

		int GetClangIncludeDepth(const std::string& line)
		{
			int depth = 0;
			for (depth = 0; depth < line.size(); depth++)
			{
				if (line[depth] != '.')
				{
					break;
				}
			}

			// Verify the next character is a space, otherwise reset the depth to zero
			if (depth < line.size() && line[depth] != ' ')
			{
				depth = 0;
			}

			return depth;
		}

		std::vector<Path> ParseMSVCIncludes(
			const Path& file,
			const std::string& output,
			std::stringstream& cleanOutput)
		{
			// Add the root file
			std::set<std::string> input;
			input.insert(file.ToString());

			std::stringstream content(output);
			std::string line;
			while (std::getline(content, line))
			{
				// TODO: Getline is dumb and uses newline on windows
				if (line[line.size() - 1] == '\r')
				{
					line.resize(line.size() - 1);
				}

				auto includeDepth = GetMSVCIncludeDepth(line);
				if (includeDepth > 0)
				{
					// Parse the file reference
					auto includePrefix = GetMSVCIncludePrefix();
					auto offset = includePrefix.size() + includeDepth;
					auto includeFile = Path(line.substr(offset));

					input.insert(includeFile.ToString());
				}
				else
				{
					// Not an include, pass along
					cleanOutput << line << "\n";
				}
			}

			auto result = std::vector<Path>();
			for (auto& value : input)
			{
				result.push_back(Path(value));
			}

			return result;
		}

		int GetMSVCIncludeDepth(const std::string& line)
		{
			int depth = 0;
			auto includePrefix = GetMSVCIncludePrefix();
			if (line.rfind(includePrefix, 0) == 0)
			{
				// Find the end of the whitespace
				int offset = includePrefix.size();
				for (; offset < line.size(); offset++)
				{
					if (line[offset] != ' ')
					{
						break;
					}
				}

				// The depth is the number of whitespaces past the prefix
				depth = offset - includePrefix.size();
			}

			return depth;
		}

		std::string_view GetMSVCIncludePrefix()
		{
			return std::string_view("Note: including file:");
		}

	private:
		Path _workingDirectory;
		std::map<int64_t, int64_t> _dependencyCounts;
		BuildHistory _buildHistory;
		BuildHistoryChecker _stateChecker;
	};
}
