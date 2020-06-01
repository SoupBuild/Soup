// <copyright file="BuildUtilities.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler
{
	/// <summary>
	/// The build utilities class
	/// </summary>
	export class BuildUtilities
	{
	public:
		/// <summary>
		/// Create a build node that will copy a file
		/// </summary>
		static Soup::Build::Extensions::BuildOperationWrapper CreateCopyFileNode(
			Soup::Build::Extensions::BuildStateWrapper& state,
			const Path& source,
			const Path& destination)
		{
			auto titleStream = std::stringstream();
			titleStream << "Copy [" << source.ToString() << "] -> [" << destination.ToString() << "]";

			auto program = Path("C:/Windows/System32/cmd.exe");
			auto workingDirectory = Path("");
			auto inputFiles = std::vector<Path>({
				source,
			});
			auto outputFiles = std::vector<Path>({
				destination,
			});

			// Build the arguments
			std::stringstream arguments;
			arguments << "/C copy /Y \"" << source.ToAlternateString() << "\" \"" << destination.ToAlternateString() << "\"";

			return Build::Extensions::BuildOperationWrapper(
				new Build::Extensions::BuildOperation(
					titleStream.str(),
					program,
					arguments.str(),
					workingDirectory,
					inputFiles,
					outputFiles));
		}

		/// <summary>
		/// Create a build node that will create a directory
		/// </summary>
		static Soup::Build::Extensions::BuildOperationWrapper CreateCreateDirectoryNode(
			Soup::Build::Extensions::BuildStateWrapper& state,
			const Path& directory)
		{
			auto titleStream = std::stringstream();
			titleStream << "MakeDir [" << directory.ToString() << "]";

			auto program = Path("C:/Windows/System32/cmd.exe");
			auto workingDirectory = Path("");
			auto inputFiles = std::vector<Path>({});
			auto outputFiles = std::vector<Path>({
				directory,
			});

			// Build the arguments
			std::stringstream arguments;
			arguments << "/C if not exist \"" << directory.ToString() << "\" mkdir \"" << directory.ToString() << "\"";

			return Build::Extensions::BuildOperationWrapper(
				new Build::Extensions::BuildOperation(
					titleStream.str(),
					program,
					arguments.str(),
					workingDirectory,
					inputFiles,
					outputFiles));
		}
	};
}
