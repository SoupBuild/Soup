﻿// <copyright file="BuildSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

// TODO: HACK TO FIX CLANG COMPILER ERROR
namespace Soup::BuildEx
{
	/// <summary>
	/// Build Task Extension interface
	/// </summary>
	export class IBuildSystem
	{
	public:
		virtual void AddIncludePath(const char* path) = 0;
		virtual void AddLibraryPath(const char* path) = 0;
	};
}


namespace Soup
{
	/// <summary>
	/// The build system implementation
	/// </summary>
	export class BuildSystem : public BuildEx::IBuildSystem
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildSystem"/> class.
		/// </summary>
		BuildSystem() :
			_includePaths()
		{
		}

		/// <summary>
		/// Add include path
		/// </summary>
		void AddIncludePath(const char* path) override final
		{
			auto includePath = Path(path);
			Log::Info("AddIncludePath: " + includePath.ToString());
			_includePaths.push_back(std::move(includePath));
		}

		/// <summary>
		/// Add library path
		/// </summary>
		void AddLibraryPath(const char* path) override final
		{
			auto libraryPath = Path(path);
			Log::Info("AddLibraryPath: " + libraryPath.ToString());
			_libraryPaths.push_back(std::move(libraryPath));
		}

		/// <summary>
		/// Get the set of added include paths
		/// </summary>
		const std::vector<Path>& GetIncludePaths()
		{
			return _includePaths;
		}

		/// <summary>
		/// Get the set of added library paths
		/// </summary>
		const std::vector<Path>& GetLibraryPaths()
		{
			return _libraryPaths;
		}

	private:
		std::vector<Path> _includePaths;
		std::vector<Path> _libraryPaths;
	};
}
