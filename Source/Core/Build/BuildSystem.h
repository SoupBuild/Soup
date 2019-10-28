// <copyright file="BuildSystem.h" company="Soup">
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
		BuildSystem()
		{
		}

		/// <summary>
		/// Add include path
		/// </summary>
		virtual void AddIncludePath(const char* path) override final
		{
			Log::Info(path);
		}
	};
}
