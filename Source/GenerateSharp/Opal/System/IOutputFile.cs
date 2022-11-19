// <copyright file="IOutputFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System
{
	using global::System;
	using global::System.IO;

	/// <summary>
	/// The output file interface
	/// Interface mainly used to allow for unit testing client code.
	/// </summary>
	public interface IOutputFile : IDisposable
	{
		/// <summary>
		/// Gets the output stream.
		/// </summary>
		Stream GetOutStream();
	}
}
