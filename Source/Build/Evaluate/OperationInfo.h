// <copyright file="OperationInfo.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Evaluate
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

	export using OperationId = int32_t;

	export class OperationInfo
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		OperationInfo() :
			Id(0),
			Title(),
			Command(),
			DeclaredInput(),
			DeclaredOutput(),
			Children(),
			DependencyCount(0),
			WasSuccessfulRun(false),
			ObservedInput(),
			ObservedOutput()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		OperationInfo(
			OperationId id,
			std::string title,
			CommandInfo command,
			std::vector<FileId> declaredInput,
			std::vector<FileId> declaredOutput,
			std::vector<OperationId> children,
			uint32_t dependencyCount) :
			Id(id),
			Title(std::move(title)),
			Command(std::move(command)),
			DeclaredInput(std::move(declaredInput)),
			DeclaredOutput(std::move(declaredOutput)),
			Children(std::move(children)),
			DependencyCount(dependencyCount),
			WasSuccessfulRun(false),
			ObservedInput(),
			ObservedOutput()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		OperationInfo(
			OperationId id,
			std::string title,
			CommandInfo command,
			std::vector<FileId> declaredInput,
			std::vector<FileId> declaredOutput,
			std::vector<OperationId> children,
			uint32_t dependencyCount,
			bool wasSuccessfulRun,
			std::vector<FileId> observedInput,
			std::vector<FileId> observedOutput) :
			Id(id),
			Title(std::move(title)),
			Command(std::move(command)),
			DeclaredInput(std::move(declaredInput)),
			DeclaredOutput(std::move(declaredOutput)),
			Children(std::move(children)),
			DependencyCount(dependencyCount),
			WasSuccessfulRun(wasSuccessfulRun),
			ObservedInput(std::move(observedInput)),
			ObservedOutput(std::move(observedOutput))
		{
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const OperationInfo& rhs) const
		{
			return Id == rhs.Id &&
				Title == rhs.Title &&
				Command == rhs.Command &&
				DeclaredInput == rhs.DeclaredInput &&
				DeclaredOutput == rhs.DeclaredOutput &&
				Children == rhs.Children &&
				DependencyCount == rhs.DependencyCount &&
				WasSuccessfulRun == rhs.WasSuccessfulRun &&
				ObservedInput == rhs.ObservedInput &&
				ObservedOutput == rhs.ObservedOutput;
		}

		OperationId Id;
		std::string Title;
		CommandInfo Command;
		std::vector<FileId> DeclaredInput;
		std::vector<FileId> DeclaredOutput;
		std::vector<OperationId> Children;
		uint32_t DependencyCount;
		bool WasSuccessfulRun;
		std::vector<FileId> ObservedInput;
		std::vector<FileId> ObservedOutput;
	};
}

namespace std
{
	template<> struct hash<Soup::Build::Evaluate::CommandInfo>
	{
		std::size_t operator()(Soup::Build::Evaluate::CommandInfo const& value) const noexcept
		{
			std::size_t hashWorkingDirectory = std::hash<std::string>{}(value.WorkingDirectory.ToString());
			std::size_t hashExecutable = std::hash<std::string>{}(value.Executable.ToString());
			std::size_t hashArguments = std::hash<std::string>{}(value.Arguments);
			return hashWorkingDirectory ^ (hashExecutable << 1) ^ (hashArguments << 2);
		}
	};
}