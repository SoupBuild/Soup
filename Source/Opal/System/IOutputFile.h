// <copyright file="IFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	/// <summary>
	/// The output file interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class IOutputFile
	{
	public:
		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		virtual std::ostream& GetStream() = 0;

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		virtual void Close() = 0;
	};
}
