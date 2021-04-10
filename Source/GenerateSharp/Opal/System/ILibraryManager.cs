// <copyright file="ILibraryManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System
{
	/// <summary>
	/// The library manager interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	public interface ILibraryManager
	{
		/// <summary>
		/// Loads a library with the provided path
		/// </summary>
		public abstract ILibrary LoadDynamicLibrary(Path library);
	}
}
