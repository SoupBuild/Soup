// <copyright file="IFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System
{
	/// <summary>
	/// The file interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	public interface IFile
	{
		/// <summary>
		/// Close the file
		/// </summary>
		void Close();
	}
}
