// <copyright file="BuildUtilities.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
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
		static std::shared_ptr<BuildGraphNode> CreateCopyFileNode(const Path& source, const Path& destination)
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

			return std::make_shared<BuildGraphNode>(
				titleStream.str(),
				std::move(program),
				arguments.str(),
				std::move(workingDirectory),
				std::move(inputFiles),
				std::move(outputFiles));
		}

		/// <summary>
		/// Create a build node that will create a directory
		/// </summary>
		static std::shared_ptr<BuildGraphNode> CreateCreateDirectoryNode(const Path& directory)
		{
			auto titleStream = std::stringstream();
			titleStream << "MakeDir [" << directory.ToString() << "]";

			auto program = Path("C:/Windows/System32/cmd.exe");
			auto workingDirectory = Path("");
			auto inputFiles = std::vector<Path>({});
			auto outputFiles = std::vector<Path>({});

			// Build the arguments
			std::stringstream arguments;
			arguments << "/C if not exist \"" << directory.ToString() << "\" mkdir \"" << directory.ToString() << "\"";

			return std::make_shared<BuildGraphNode>(
				titleStream.str(),
				std::move(program),
				arguments.str(),
				std::move(workingDirectory),
				std::move(inputFiles),
				std::move(outputFiles));
		}
	};
}
