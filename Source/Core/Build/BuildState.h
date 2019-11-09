// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
	export class FileInfo
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="FileInfo"/> class.
		/// </summary>
		FileInfo() :
			File(),
			Includes()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="FileInfo"/> class.
		/// </summary>
		FileInfo(Path file, std::vector<Path> includes) :
			File(std::move(file)),
			Includes(std::move(includes))
		{
		}

		Path File;
		std::vector<Path> Includes;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const FileInfo& rhs) const
		{
			return File == rhs.File &&
				Includes == rhs.Includes;
		}

		bool operator !=(const FileInfo& rhs) const
		{
			return !(*this == rhs);
		}
	};

	struct FileInfo_LessThan
	{
		bool operator() (const FileInfo& lhs, const FileInfo& rhs) const
		{
			return lhs.File < rhs.File;
		}
	};

	export class BuildState
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildState"/> class.
		/// </summary>
		BuildState() :
			KnownFiles()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="BuildState"/> class.
		/// </summary>
		BuildState(std::vector<FileInfo> knownFiles) :
			KnownFiles(std::move(knownFiles))
		{
		}

		std::vector<FileInfo> KnownFiles;

		/// <summary>
		/// Recursively build up the closure of all included files
		/// from the build state
		/// </summary>
		bool TryBuildIncludeClosure(
			const Path& sourceFile,
			std::vector<Path>& closure)
		{
			closure.clear();
			if (!TryBuildIncludeClosure(
				std::vector<Path>({ sourceFile }),
				closure))
			{
				closure.clear();
				return false;
			}
			else
			{
				return true;
			}
		}

		/// <summary>
		/// Update the build state for the provided files
		/// </summary>
		void UpdateIncludeTree(const std::vector<HeaderInclude>& includeTree)
		{
			// Flatten out the tree
			auto activeSet = std::set<FileInfo, FileInfo_LessThan>(
				std::make_move_iterator(KnownFiles.begin()),
				std::make_move_iterator(KnownFiles.end()));
			KnownFiles.clear();

			UpdateIncludes(activeSet, includeTree);

			// Convert the set back to a vector
			KnownFiles = std::vector<FileInfo>(
				std::make_move_iterator(activeSet.begin()),
				std::make_move_iterator(activeSet.end()));
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const BuildState& rhs) const
		{
			return KnownFiles == rhs.KnownFiles;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const BuildState& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		/// <summary>
		/// Internal implentation
		/// </summary>
		bool TryBuildIncludeClosure(
			const std::vector<Path>& sourceFiles,
			std::vector<Path>& closure)
		{
			for (auto& file : sourceFiles)
			{
				auto fileInfoResult = std::find_if(
					KnownFiles.begin(),
					KnownFiles.end(),
					[&file](const FileInfo& fileInfo) { return fileInfo.File == file; });
				if (fileInfoResult != KnownFiles.end())
				{
					// Find all of the files that do not already exist in the closure
					auto& includes = fileInfoResult->Includes;
					auto newIncludes = std::vector<Path>();
					for (auto& include : includes)
					{
						if (std::find(closure.begin(), closure.end(), include) == closure.end())
						{
							newIncludes.push_back(include);
						}
					}

					// Add all the new files to the closure
					std::copy(
						newIncludes.begin(),
						newIncludes.end(),
						std::back_inserter(closure));

					// Build up the child includes
					if (!TryBuildIncludeClosure(newIncludes, closure))
					{
						// Propagate the failed result
						return false;
					}
				}
				else
				{
					Log::Info("Missing file info: " + file.ToString());
					return false;
				}
			}

			return true;
		}

		/// <summary>
		/// Update the build state for the provided files
		/// </summary>
		static void UpdateIncludes(
			std::set<FileInfo, FileInfo_LessThan>& activeSet,
			const std::vector<HeaderInclude>& level)
		{
			for (auto& current : level)
			{
				// Create the FileInfo
				auto info = FileInfo();
				info.File = current.Filename;
				for (auto& include : current.Includes)
				{
					info.Includes.push_back(include.Filename);
				}

				// Remove previous entry if exists
				auto existingFileInfo = activeSet.find(info);
				if (existingFileInfo != activeSet.end())
				{
					activeSet.erase(existingFileInfo);
				}

				// Add the file info
				activeSet.insert(std::move(info));

				// Recurse to the children
				UpdateIncludes(activeSet, current.Includes);
			}
		}
	};
}
