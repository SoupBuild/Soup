// <copyright file="BuildHistory.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "CompileResult.h"

namespace Soup::Build
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

	export class BuildHistory
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildHistory"/> class.
		/// </summary>
		BuildHistory() :
			_knownFiles(),
			_fastLookup()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="BuildHistory"/> class.
		/// </summary>
		BuildHistory(std::vector<FileInfo> knownFiles) :
			_knownFiles(std::move(knownFiles)),
			_fastLookup()
		{
			BuildFastLookupDictionary();
		}

		/// <summary>
		/// Get the known files list
		/// </summary>
		const std::vector<FileInfo>& GetKnownFiles() const
		{
			return _knownFiles;
		}

		/// <summary>
		/// Recursively build up the closure of all included files
		/// from the build state
		/// </summary>
		bool TryBuildIncludeClosure(
			const std::vector<Path>& sourceFiles,
			std::vector<Path>& closure)
		{
			closure.clear();
			auto closureSet = std::unordered_set<std::string>();
			if (TryBuildIncludeClosure(
				sourceFiles,
				closureSet))
			{
				// Convert the set to a vector output
				for (auto& file : closureSet)
				{
					closure.push_back(Path(file));
				}

				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Update the build state for the provided files
		/// </summary>
		void UpdateIncludeTree(const std::vector<HeaderInclude>& includeTree)
		{
			// Flatten out the tree
			auto activeSet = std::set<FileInfo, FileInfo_LessThan>(
				std::make_move_iterator(_knownFiles.begin()),
				std::make_move_iterator(_knownFiles.end()));
			_knownFiles.clear();

			UpdateIncludes(activeSet, includeTree);

			// Convert the set back to a vector
			_knownFiles = std::vector<FileInfo>(
				std::make_move_iterator(activeSet.begin()),
				std::make_move_iterator(activeSet.end()));

			// Ensure the dictionary is up to date
			BuildFastLookupDictionary();
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const BuildHistory& rhs) const
		{
			return _knownFiles == rhs._knownFiles;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const BuildHistory& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		void BuildFastLookupDictionary()
		{
			_fastLookup.clear();
			for (auto& info : _knownFiles)
			{
				_fastLookup.emplace(info.File.ToString(), info);
			}
		}

		/// <summary>
		/// Internal implentation
		/// </summary>
		bool TryBuildIncludeClosure(
			const std::vector<Path>& sourceFiles,
			std::unordered_set<std::string>& closure)
		{
			for (auto& file : sourceFiles)
			{
				auto fileInfoResult = _fastLookup.find(file.ToString());
				if (fileInfoResult != _fastLookup.end())
				{
					// Find all of the files that do not already exist in the closure
					auto& includes = fileInfoResult->second.Includes;
					auto newIncludes = std::vector<Path>();
					for (auto& include : includes)
					{
						auto insertResult = closure.insert(include.ToString());
						if (insertResult.second)
						{
							newIncludes.push_back(include);
						}
					}

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

	private:
		std::unordered_map<std::string, FileInfo&> _fastLookup;
		std::vector<FileInfo> _knownFiles;
	};
}
