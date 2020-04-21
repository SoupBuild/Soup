// <copyright file="IConsoleInputStream.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::IO
{
	/// <summary>
	/// The console input stream interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class IConsoleInputStream
	{
	public:
		/// <summary>
		/// Read a line from the stream
		/// </summary>
		virtual std::string ReadLine() = 0;

		/// <summary>
		/// Read a password from the stream
		/// </summary>
		virtual std::string ReadPassword() = 0;
	};
}
