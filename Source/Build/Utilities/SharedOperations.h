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

			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();

			auto program = moduleFolder + Path("writefile.exe");
			auto inputFiles = std::vector<Path>({});
			auto outputFiles = std::vector<Path>({
				destination,
			});

			// Build the arguments
			std::stringstream arguments;
			arguments << "\"" << destination.ToString() << "\" \"";
			EscapeString(content, arguments);
			arguments << "\"";

			return BuildOperation(
				titleStream.str(),
				workingDirectory,
				program,
				arguments.str(),
				std::move(inputFiles),
				std::move(outputFiles));
		}

	private:
		// TODO: Should not be doing my own raw UTF-8 manipulation, find a library or create one in Opal
		static void EscapeString(std::string_view value, std::ostream& stream)
		{
			for (auto character : value)
			{
				switch (character)
				{
				case '"':
					stream << "\\\"";
					break;
				case '\\':
					stream << "\\\\";
					break;
				case '\b':
					stream << "\\b";
					break;
				case '\f':
					stream << "\\f";
					break;
				case '\n':
					stream << "\\n";
					break;
				case '\r':
					stream << "\\r";
					break;
				case '\t':
					stream << "\\t";
					break;
				default:
					if ('\x00' <= character && character <= '\x1f')
					{
						throw std::runtime_error("Unicode escape characters not supported yet...");
					}
					else
					{
						stream << character;
					}

					break;
				}
			}
		}
	};
}
