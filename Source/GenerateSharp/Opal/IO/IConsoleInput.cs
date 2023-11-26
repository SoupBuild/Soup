// <copyright file="IConsoleInput.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.IO;

/// <summary>
/// The console input stream interface
/// Interface mainly used to allow for unit testing client code.
/// </summary>
public interface IConsoleInput
{
	/// <summary>
	/// Read a line from the stream.
	/// </summary>
	public string ReadLine();

	/// <summary>
	/// Read a password from the stream.
	/// </summary>
	public string ReadPassword();
}
