// <copyright file="IInputFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.IO;

namespace Opal.System
{
	/// <summary>
	/// The input file interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	public interface IInputFile : IDisposable
	{
		/// <summary>
		/// Gets the input stream
		/// </summary>
		Stream GetInStream();
	}
}
