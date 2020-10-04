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
		/// Create a build operation that will copy a file
		/// </summary>
		static Soup::Build::Utilities::BuildOperationWrapper CreateCopyFileOperation(
			Soup::Build::Utilities::BuildStateWrapper& state,
			const Path& source,
			const Path& destination)
		{
			auto titleStream = std::stringstream();
			titleStream << "Copy [" << source.ToString() << "] -> [" << destination.ToString() << "]";

			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();

			auto program = moduleFolder + Path("copy.exe");
			auto workingDirectory = Path("");
			auto inputFiles = std::vector<Path>({
				source,
			});
			auto outputFiles = std::vector<Path>({
				destination,
			});

			// Build the arguments
			std::stringstream arguments;
			arguments << "\"" << source.ToAlternateString() << "\" \"" << destination.ToAlternateString() << "\"";

			return Build::Utilities::BuildOperationWrapper(
				new Build::Utilities::BuildOperation(
					titleStream.str(),
					program,
					arguments.str(),
					workingDirectory,
					inputFiles,
					outputFiles));
		}

		/// <summary>
		/// Create a build operation that will create a directory
		/// </summary>
		static Soup::Build::Utilities::BuildOperationWrapper CreateCreateDirectoryOperation(
			Soup::Build::Utilities::BuildStateWrapper& state,
			const Path& directory)
		{
			auto titleStream = std::stringstream();
			titleStream << "MakeDir [" << directory.ToString() << "]";

			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();

			auto program = moduleFolder + Path("mkdir.exe");
			auto workingDirectory = Path("");
			auto inputFiles = std::vector<Path>({});
			auto outputFiles = std::vector<Path>({
				directory,
			});

			// Build the arguments
			std::stringstream arguments;
			arguments << "\"" << directory.ToString() << "\"";

			return Build::Utilities::BuildOperationWrapper(
				new Build::Utilities::BuildOperation(
					titleStream.str(),
					program,
					arguments.str(),
					workingDirectory,
					inputFiles,
					outputFiles));
		}
	};
}
