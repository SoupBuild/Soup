// <copyright file="BuildOperation.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildOperationList.h"
#include "StringList.h"

namespace Soup::Build::Extensions
{
	/// <summary>
	/// A graph node that represents a single operation in the build
	/// </summary>
	export class BuildOperation : public Memory::ReferenceCounted<IBuildOperation>
	{
	public:
		BuildOperation(
			std::string title,
			const Path& program,
			std::string arguments,
			const Path& workingDirectory,
			const std::vector<Path>& inputFiles,
			const std::vector<Path>& outputFiles) :
			_title(std::move(title)),
			_program(program.ToString()),
			_arguments(std::move(arguments)),
			_workingDirectory(workingDirectory.ToString()),
			_inputFiles(inputFiles),
			_outputFiles(outputFiles),
			_children()
		{
		}

		BuildOperation(
			std::string title,
			const Path& program,
			std::string arguments,
			const Path& workingDirectory,
			const std::vector<Path>& inputFiles,
			const std::vector<Path>& outputFiles,
			std::vector<Memory::Reference<BuildOperation>> children) :
			_title(std::move(title)),
			_program(program.ToString()),
			_arguments(std::move(arguments)),
			_workingDirectory(workingDirectory.ToString()),
			_inputFiles(inputFiles),
			_outputFiles(outputFiles),
			_children(std::move(children))
		{
			// TODO: Verify circular references in debug build
		}

		const char* GetTitle() const noexcept override final
		{
			return _title.c_str();
		}

		const char* GetExecutable() const noexcept override final
		{
			return _program.c_str();
		}

		const char* GetArguments() const noexcept override final
		{
			return _arguments.c_str();
		}

		const char* GetWorkingDirectory() const noexcept override final
		{
			return _workingDirectory.c_str();
		}

		const IList<const char*>& GetInputFileList() const noexcept override final
		{
			return _inputFiles;
		}
		
		const IList<const char*>& GetOutputFileList() const noexcept override final
		{
			return _outputFiles;
		}

		IList<IBuildOperation*>& GetChildList() noexcept override final
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

		const std::vector<Memory::Reference<BuildOperation>>& GetChildren() const
		{
			return _children.GetValues();
		}

	private:
		std::string _title;
		std::string _program;
		std::string _arguments;
		std::string _workingDirectory;
		StringList _inputFiles;
		StringList _outputFiles;
		BuildOperationList _children;
	};
}
