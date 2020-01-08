// <copyright file="BuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Build/Runner/BuildHistory.h"
#include "Build/System/BuildGraphNode.h"

namespace Soup::Build
{
	/// <summary>
	/// The build task
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

		void Execute(const std::vector<Memory::Reference<BuildGraphNode>>& nodes, bool forceBuild)
		{
			// Load the previous build state if performing an incremental build
			if (!forceBuild)
			{
				Log::Diag("Loading previous build state");
				if (!BuildHistoryManager::TryLoadState(_workingDirectory, _buildHistory))
				{
					Log::Info("No previous state found, full rebuild required");
					_buildHistory = BuildHistory();
					forceBuild = true;
				}
			}

			// Build the initial node dependency set to
			// ensure nodes are built in the correct order 
			// and that there are no cycles
			auto emptyParentSet = std::set<int>();
			BuildDependencies(nodes, emptyParentSet);

			// Run all build nodes in the correct order with incremental build checks
			CheckExecuteNodes(nodes, forceBuild);

			Log::Info("Saving updated build state");
			BuildHistoryManager::SaveState(_workingDirectory, _buildHistory);

			Log::HighPriority("Done");
		}

	private:
		/// <summary>
		/// Build dependencies
		/// </summary>
		void BuildDependencies(
			const std::vector<Memory::Reference<BuildGraphNode>>& nodes,
			const std::set<int>& parentSet)
		{
			for (auto& node : nodes)
			{
				// Make sure there are no cycles
				if (parentSet.contains(node->GetId()))
					throw std::runtime_error("A build node graph must be acyclic.");
				
				// Check if the node was already a child from a different path
				auto currentNodeSearch = _dependencyCounts.find(node->GetId());
				if (currentNodeSearch != _dependencyCounts.end())
				{
					// Increment the dependency count
					currentNodeSearch->second++;
				}
				else
				{
					// Insert a new entry with a single count
					_dependencyCounts.emplace(node->GetId(), 1);
					
					// Recurse to children only the first time we see a node
					auto updatedParentSet = parentSet;
					updatedParentSet.insert(node->GetId());
					BuildDependencies(node->GetChildren(), updatedParentSet);
				}
			}
		}

		/// <summary>
		/// Execute the collection of build nodes
		/// </summary>
		void CheckExecuteNodes(
			const std::vector<Memory::Reference<BuildGraphNode>>& nodes,
			bool forceBuild)
		{
			for (auto& node : nodes)
			{
				// Check if the node was already a child from a different path
				auto currentNodeSearch = _dependencyCounts.find(node->GetId());
				if (currentNodeSearch != _dependencyCounts.end())
				{
					auto remainingCount = --currentNodeSearch->second;
					if (remainingCount == 0)
					{
						ExecuteNode(*node, forceBuild);
					}
					else
					{
						// This node will be executed from a different path
					}
				}
				else
				{
					throw std::runtime_error("A node id was missing from the dependency collection.");
				}
			}
		}

		/// <summary>
		/// Execute a single build node and all of its children
		/// </summary>
		void ExecuteNode(
			const BuildGraphNode& node,
			bool forceBuild)
		{
			bool buildRequired = forceBuild;
			if (!forceBuild)
			{
				// Check if each source file is out of date and requires a rebuild
				Log::Diag("Check for updated source");
				
				// Try to build up the closure of include dependencies
				const auto& inputFiles = node.GetInputFiles();
				if (!inputFiles.empty())
				{
					auto inputClosure = std::vector<Path>();

					// TODO: Is this how we want to handle no input nodes?
					// If there are source files to the node check their build state
					for (auto& inputFile : inputFiles)
					{
						// Build the input closure for all source files
						auto inputFilePath = Path(inputFile);
						if (inputFilePath.GetFileExtension() == ".cpp")
						{
							if (!_buildHistory.TryBuildIncludeClosure(inputFilePath, inputClosure))
							{
								// Could not determine the set of input files, not enough info to perform incremental build
								buildRequired = true;
								break;
							}
						}
					}

					// If we were able to load all of the build history then perform the timestamp checks
					if (!buildRequired)
					{
						// Include the source files itself
						inputClosure.insert(inputClosure.end(), inputFiles.begin(), inputFiles.end());

						// Load the output files
						auto outputFiles = std::vector<Path>();
						for (auto& file : node.GetOutputFiles())
							outputFiles.push_back(Path(file));

						// Check if any of the input files have changed since last build
						if (_stateChecker.IsOutdated(
							outputFiles,
							inputClosure,
							Path(node.GetWorkingDirectory())))
						{
							// The file or a dependency has changed
							buildRequired = true;
						}
						else
						{
							Log::Info("Up to date");
						}
					}
				}
			}

			if (buildRequired)
			{
				Log::HighPriority(node.GetTitle());
				auto program = Path(node.GetProgram());
				auto message = "Execute: " + program.ToString() + " " + node.GetArguments();
				Log::Diag(message);
				auto result = System::IProcessManager::Current().Execute(
					program,
					node.GetArguments(),
					Path(node.GetWorkingDirectory()));

				// Try parse includes if available
				auto cleanOutput = std::stringstream();
				auto headerIncludes = std::vector<HeaderInclude>();
				if (TryParsesHeaderIncludes(node, result.StdOut, headerIncludes, cleanOutput))
				{
					// Save off the build history for future builds
					_buildHistory.UpdateIncludeTree(headerIncludes);

					// Replace the output string with the clean version
					result.StdOut = cleanOutput.str();
				}

				if (!result.StdOut.empty())
				{
					// Upgrade output to a warning if the command fails
					if (result.ExitCode != 0)
						Log::Warning(result.StdOut);
					else
						Log::Info(result.StdOut);
				}

				// If there was any error output then the build failed
				// TODO: Find warnings + errors
				if (!result.StdErr.empty())
				{
					Log::Error(result.StdErr);
				}

				if (result.ExitCode != 0)
				{
					throw std::runtime_error("Compiler Object Error: " + std::to_string(result.ExitCode));
				}
			}

			// Recursively build all of the node chilren
			// Note: Force build if this node was built
			CheckExecuteNodes(node.GetChildren(), buildRequired);
		}

		bool TryParsesHeaderIncludes(
			const BuildGraphNode& node,
			const std::string& output,
			std::vector<HeaderInclude>& headerIncludes,
			std::stringstream& cleanOutput)
		{
			// Check for any cpp input files
			auto program = Path(node.GetProgram());
			for (auto& inputFile : node.GetInputFiles())
			{
				auto inputFilePath = Path(inputFile);
				if (inputFilePath.GetFileExtension() == ".cpp")
				{
					// Parse known compiler output
					if (program.GetFileName() == "cl.exe")
					{
						headerIncludes = ParseMSVCIncludes(inputFilePath, output, cleanOutput);
						return true;
					}
					else if (program.GetFileName() == "clang++.exe")
					{
						headerIncludes = ParseMSVCIncludes(inputFilePath, output, cleanOutput);
						return true;
					}
				}
			}

			headerIncludes = std::vector<HeaderInclude>();
			return false;
		}

		std::vector<HeaderInclude> ParseClangIncludes(
			const Path& file,
			const std::string& output,
			std::stringstream& cleanOutput)
		{
			// Add the root file
			std::stack<HeaderInclude> current;
			current.push(HeaderInclude(file));

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

					// Ensure we are at the correct depth
					while (includeDepth < current.size())
					{
						// Remove the top file and push it onto its parent
						auto previous = std::move(current.top());
						current.pop();
						current.top().Includes.push_back(std::move(previous));
					}

					// Ensure we do not try to go up more than one level at a time
					if (includeDepth > current.size() + 1)
						throw std::runtime_error("Missing an include level.");

					current.push(HeaderInclude(includeFile));
				}
				else
				{
					// Not an include, pass along
					cleanOutput << line << "\n";
				}
			}

			// Ensure we are at the top level
			while (1 < current.size())
			{
				// Remove the top file and push it onto its parent
				auto previous = std::move(current.top());
				current.pop();
				current.top().Includes.push_back(std::move(previous));
			}

			return std::vector<HeaderInclude>({ std::move(current.top()) });
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

		std::vector<HeaderInclude> ParseMSVCIncludes(
			const Path& file,
			const std::string& output,
			std::stringstream& cleanOutput)
		{
			// Add the root file
			std::stack<HeaderInclude> current;
			current.push(HeaderInclude(file));

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

					// Ensure we are at the correct depth
					while (includeDepth < current.size())
					{
						// Remove the top file and push it onto its parent
						auto previous = std::move(current.top());
						current.pop();
						current.top().Includes.push_back(std::move(previous));
					}

					// Ensure we do not try to go up more than one level at a time
					if (includeDepth > current.size() + 1)
						throw std::runtime_error("Missing an include level.");

					current.push(HeaderInclude(includeFile));
				}
				else
				{
					// Not an include, pass along
					cleanOutput << line << "\n";
				}
			}

			// Ensure we are at the top level
			while (1 < current.size())
			{
				// Remove the top file and push it onto its parent
				auto previous = std::move(current.top());
				current.pop();
				current.top().Includes.push_back(std::move(previous));
			}

			return std::vector<HeaderInclude>({ std::move(current.top()) });
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
		std::map<int, int> _dependencyCounts;
		BuildHistory _buildHistory;
		BuildHistoryChecker _stateChecker;
	};
}
