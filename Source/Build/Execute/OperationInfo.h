// <copyright file="OperationInfo.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute
{
	export class CommandInfo
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="CommandInfo"/> class.
		/// </summary>
		CommandInfo() :
			WorkingDirectory(),
			Executable(),
			Arguments()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="CommandInfo"/> class.
		/// </summary>
		CommandInfo(
			Path workingDirectory,
			Path executable,
			std::string arguments) :
			WorkingDirectory(std::move(workingDirectory)),
			Executable(std::move(executable)),
			Arguments(std::move(arguments))
		{
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const CommandInfo& rhs) const
		{
			return WorkingDirectory == rhs.WorkingDirectory &&
				Executable == rhs.Executable &&
				Arguments == rhs.Arguments;
		}

		Path WorkingDirectory;
		Path Executable;
		std::string Arguments;
	};

	export class OperationInfo
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		OperationInfo() :
			Command(),
			Input(),
			Output()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		OperationInfo(
			CommandInfo command,
			std::vector<FileId> input,
			std::vector<FileId> output) :
			Command(std::move(command)),
			Input(std::move(input)),
			Output(std::move(output))
		{
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const OperationInfo& rhs) const
		{
			return Command == rhs.Command &&
				Input == rhs.Input &&
				Output == rhs.Output;
		}

		CommandInfo Command;
		std::vector<FileId> Input;
		std::vector<FileId> Output;
	};
}

namespace std
{
	template<> struct hash<Soup::Build::Execute::CommandInfo>
	{
		std::size_t operator()(Soup::Build::Execute::CommandInfo const& value) const noexcept
		{
			std::size_t hashWorkingDirectory = std::hash<std::string>{}(value.WorkingDirectory.ToString());
			std::size_t hashExecutable = std::hash<std::string>{}(value.Executable.ToString());
			std::size_t hashArguments = std::hash<std::string>{}(value.Arguments);
			return hashWorkingDirectory ^ (hashExecutable << 1) ^ (hashArguments << 2);
		}
	};
}