// <copyright file="BuildResult.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Utilities
{
	export class BuildOperation
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildOperation"/> class.
		/// </summary>
		BuildOperation() :
			Title(),
			WorkingDirectory(),
			Executable(),
			Arguments(),
			DeclaredInput(),
			DeclaredOutput()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="BuildOperation"/> class.
		/// </summary>
		BuildOperation(
			std::string title,
			Path workingDirectory,
			Path executable,
			std::string arguments,
			std::vector<Path> declaredInput,
			std::vector<Path> declaredOutput) :
			Title(std::move(title)),
			WorkingDirectory(std::move(workingDirectory)),
			Executable(std::move(executable)),
			Arguments(std::move(arguments)),
			DeclaredInput(std::move(declaredInput)),
			DeclaredOutput(std::move(declaredOutput))
		{
		}

		std::string Title;
		Path WorkingDirectory;
		Path Executable;
		std::string Arguments;
		std::vector<Path> DeclaredInput;
		std::vector<Path> DeclaredOutput;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const BuildOperation& rhs) const
		{
			return Title == rhs.Title &&
				WorkingDirectory == rhs.WorkingDirectory &&
				Executable == rhs.Executable &&
				Arguments == rhs.Arguments &&
				DeclaredInput == rhs.DeclaredInput &&
				DeclaredOutput == rhs.DeclaredOutput;
		}

		bool operator !=(const BuildOperation& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
