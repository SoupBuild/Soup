// <copyright file="SharedOperations.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Utilities
{
	/// <summary>
	/// The Shared Operations class
	/// </summary>
	export class SharedOperations
	{
	public:
		/// <summary>
		/// Create a build operation that will copy a file
		/// </summary>
		static BuildOperation CreateCopyFileOperation(
			const Path& workingDirectory,
			const Path& source,
			const Path& destination)
		{
			auto titleStream = std::stringstream();
			titleStream << "Copy [" << source.ToString() << "] -> [" << destination.ToString() << "]";

			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();

			auto program = moduleFolder + Path("copy.exe");
			auto inputFiles = std::vector<Path>({
				source,
			});
			auto outputFiles = std::vector<Path>({
				destination,
			});

			// Build the arguments
			std::stringstream arguments;
			arguments << "\"" << source.ToString() << "\" \"" << destination.ToString() << "\"";

			return BuildOperation(
				titleStream.str(),
				workingDirectory,
				program,
				arguments.str(),
				inputFiles,
				outputFiles);
		}

		/// <summary>
		/// Create a build operation that will create a directory
		/// </summary>
		static BuildOperation CreateCreateDirectoryOperation(
			const Path& workingDirectory,
			const Path& directory)
		{
			if (directory.HasFileName())
				throw std::runtime_error("Cannot create a directory with a filename.");

			auto titleStream = std::stringstream();
			titleStream << "MakeDir [" << directory.ToString() << "]";

			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();

			auto program = moduleFolder + Path("mkdir.exe");
			auto inputFiles = std::vector<Path>({});
			auto outputFiles = std::vector<Path>({
				directory,
			});

			// Build the arguments
			std::stringstream arguments;
			arguments << "\"" << directory.ToString() << "\"";

			return BuildOperation(
				titleStream.str(),
				workingDirectory,
				program,
				arguments.str(),
				inputFiles,
				outputFiles);
		}

		/// <summary>
		/// Create a build operation that will write the content to a file
		/// </summary>
		static BuildOperation CreateWriteFileOperation(
			const Path& workingDirectory,
			const Path& destination,
			std::string_view content)
		{
			if (!destination.HasFileName())
				throw std::runtime_error("Cannot create a file with from a directory.");

			auto titleStream = std::stringstream();
			titleStream << "WriteFile [" << destination.ToString() << "]";

			// Create the fake write file executable that will be executed in process
			auto program = Path("writefile.exe");
			auto inputFiles = std::vector<Path>({});
			auto outputFiles = std::vector<Path>({
				destination,
			});

			// Build the arguments
			std::stringstream arguments;
			arguments << "\"" << destination.ToString() << "\" \"";
			arguments << content << "\"";

			return BuildOperation(
				titleStream.str(),
				workingDirectory,
				program,
				arguments.str(),
				std::move(inputFiles),
				std::move(outputFiles));
		}
	};
}
