// <copyright file="BuildGraphNode.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "StringList.h"
#include "ReferenceList.h"

namespace Soup::Build
{
	/// <summary>
	/// A graph node that represents a single operation in the build
	/// </summary>
	export class BuildGraphNode : public Memory::ReferenceCounted<IGraphNode>
	{
	private:
		inline static std::atomic<int> UniqueId = 0;

	public:
		BuildGraphNode() :
			_id(++UniqueId)
		{
		}

		BuildGraphNode(
			std::string title,
			std::string program,
			std::string arguments,
			std::string workingDirectory,
			std::vector<std::string> inputFiles,
			std::vector<std::string> outputFiles) :
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
			std::string program,
			std::string arguments,
			std::string workingDirectory,
			std::vector<std::string> inputFiles,
			std::vector<std::string> outputFiles,
			std::vector<Memory::Reference<BuildGraphNode>> children) :
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

		/// <summary>
		/// Interface accessors
		/// </summary>
		int64_t GetId() const noexcept override final
		{
			return _id;
		}

		const char* GetTitle() const noexcept override final
		{
			return _title.c_str();
		}

		OperationResult TrySetTitle(const char* value) noexcept override final
		{
			try
			{
				_title = value;
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		const char* GetProgram() const noexcept override final
		{
			return _program.c_str();
		}

		OperationResult TrySetProgram(const char* value) noexcept override final
		{
			try
			{
				_program = value;
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		const char* GetArguments() const noexcept override final
		{
			return _arguments.c_str();
		}

		OperationResult TrySetArguments(const char* value) noexcept override final
		{
			try
			{
				_arguments = value;
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		const char* GetWorkingDirectory() const noexcept override final
		{
			return _workingDirectory.c_str();
		}

		OperationResult TrySetWorkingDirectory(const char* value) noexcept override final
		{
			try
			{
				_workingDirectory = value;
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		IList<const char*>& GetInputFileList() noexcept override final
		{
			return _inputFiles;
		}
		
		IList<const char*>& GetOutputFileList() noexcept override final
		{
			return _outputFiles;
		}

		IList<IGraphNode*>& GetChildList() noexcept override final
		{
			return _children;
		}

		/// <summary>
		/// Internal accessors
		/// </summary>
		const std::vector<std::string>& GetInputFiles() const
		{
			return _inputFiles.GetValues();
		}

		const std::vector<std::string>& GetOutputFiles() const
		{
			return _outputFiles.GetValues();
		}

		const std::vector<Memory::Reference<BuildGraphNode>>& GetChildren() const
		{
			return _children.GetValues();
		}

	private:
		int64_t _id;
		std::string _title;
		std::string _program;
		std::string _arguments;
		std::string _workingDirectory;
		StringList _inputFiles;
		StringList _outputFiles;
		ReferenceList<BuildGraphNode, IGraphNode> _children;
	};
}
