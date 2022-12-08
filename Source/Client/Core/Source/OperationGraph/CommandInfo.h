// <copyright file="OperationInfo.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
using namespace std::chrono_literals;

namespace Soup::Core
{
	/// <summary>
	/// The core command information that describes exactly what to run when evaluating an operation.
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class CommandInfo
	{
	public:
		Path WorkingDirectory;
		Path Executable;
		std::string Arguments;

	public:
		CommandInfo() :
			WorkingDirectory(),
			Executable(),
			Arguments()
		{
		}

		CommandInfo(
			Path workingDirectory,
			Path executable,
			std::string arguments) :
			WorkingDirectory(std::move(workingDirectory)),
			Executable(std::move(executable)),
			Arguments(std::move(arguments))
		{
		}

		bool operator ==(const CommandInfo& rhs) const
		{
			return WorkingDirectory == rhs.WorkingDirectory &&
				Executable == rhs.Executable &&
				Arguments == rhs.Arguments;
		}
	};
}

namespace std
{
	template<> struct hash<Soup::Core::CommandInfo>
	{
		std::size_t operator()(Soup::Core::CommandInfo const& value) const noexcept
		{
			std::size_t hashWorkingDirectory = std::hash<std::string>{}(value.WorkingDirectory.ToString());
			std::size_t hashExecutable = std::hash<std::string>{}(value.Executable.ToString());
			std::size_t hashArguments = std::hash<std::string>{}(value.Arguments);
			return hashWorkingDirectory ^ (hashExecutable << 1) ^ (hashArguments << 2);
		}
	};
}