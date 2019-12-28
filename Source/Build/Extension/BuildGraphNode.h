// <copyright file="BuildGraphNode.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// A graph node that represents a single operation in the build
	/// </summary>
	export class BuildGraphNode
	{
	private:
		inline static std::atomic<int> UniqueId = 0;

	public:
		BuildGraphNode(
			std::string title,
			Path program,
			std::string arguments,
			Path workingDirectory,
			std::vector<Path> inputFiles,
			std::vector<Path> outputFiles) :
			_id(UniqueId++),
			_title(std::move(title)),
			_program(std::move(program)),
			_arguments(std::move(arguments)),
			_workingDirectory(std::move(workingDirectory)),
			_inputFiles(std::move(inputFiles)),
			_outputFiles(std::move(outputFiles)),
			_children()
		{
		}

		BuildGraphNode(
			std::string title,
			Path program,
			std::string arguments,
			Path workingDirectory,
			std::vector<Path> inputFiles,
			std::vector<Path> outputFiles,
			std::vector<std::shared_ptr<BuildGraphNode>> children) :
			_id(UniqueId++),
			_title(std::move(title)),
			_program(std::move(program)),
			_arguments(std::move(arguments)),
			_workingDirectory(std::move(workingDirectory)),
			_inputFiles(std::move(inputFiles)),
			_outputFiles(std::move(outputFiles)),
			_children(std::move(children))
		{
		}

		static void AddLeafChild(
			std::shared_ptr<BuildGraphNode>& parent,
			std::shared_ptr<BuildGraphNode>& child)
		{
			if (parent == child)
			{
				// TODO: Clean up, the node has been added through a different path
				return;
			}

			if (parent->_children.empty())
			{
				// Add the new leaf node
				parent->_children.push_back(child);
			}
			else
			{
				// Continue on to the current parents children
				AddLeafChild(parent->_children, child);
			}
		}

		static void AddLeafChild(
			std::vector<std::shared_ptr<BuildGraphNode>>& parents,
			std::shared_ptr<BuildGraphNode>& child)
		{
			for (auto& parent : parents)
			{
				AddLeafChild(parent, child);
			}
		}

		static void AddLeafChildren(
			std::shared_ptr<BuildGraphNode>& parent,
			std::vector<std::shared_ptr<BuildGraphNode>>& children)
		{
			if (parent->_children.empty())
			{
				// Add the new leaf node
				parent->_children.insert(parent->_children.end(), children.begin(), children.end());
			}
			else
			{
				// Continue on to the current parents children
				AddLeafChildren(parent->_children, children);
			}
		}

		static void AddLeafChildren(
			std::vector<std::shared_ptr<BuildGraphNode>>& parents,
			std::vector<std::shared_ptr<BuildGraphNode>>& children)
		{
			for (auto& parent : parents)
			{
				AddLeafChildren(parent, children);
			}
		}

		int GetId() const
		{
			return _id;
		}

		const std::string& GetTitle() const
		{
			return _title;
		}

		const Path& GetProgram() const
		{
			return _program;
		}

		const std::string& GetArguments() const
		{
			return _arguments;
		}

		const Path& GetWorkingDirectory() const
		{
			return _workingDirectory;
		}

		const std::vector<Path>& GetInputFiles() const
		{
			return _inputFiles;
		}
		
		const std::vector<Path>& GetOutputFiles() const
		{
			return _outputFiles;
		}

		const std::vector<std::shared_ptr<BuildGraphNode>>& GetChildren() const
		{
			return _children;
		}

	private:
		int _id;
		std::string _title;
		Path _program;
		std::string _arguments;
		Path _workingDirectory;
		std::vector<Path> _inputFiles;
		std::vector<Path> _outputFiles;
		std::vector<std::shared_ptr<BuildGraphNode>> _children;
	};
}
