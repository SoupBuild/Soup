// <copyright file="ILibraryManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal.System
{
	/// <summary>
	/// The library manager interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	public abstract class ILibraryManager
	{
		/// <summary>
		/// Gets the current active library manager
		/// </summary>
		public static ILibraryManager Current()
		{
			if (_current == null)
				throw new InvalidOperationException("No library manager implementation registered.");
			return _current;
		}

		/// <summary>
		/// Register a new active library manager
		/// </summary>
		public static void Register(ILibraryManager? value)
		{
			_current = value;
		}

		/// <summary>
		/// Loads a library with the provided path
		/// </summary>
		public abstract ILibrary LoadDynamicLibrary(Path library);

		private static ILibraryManager? _current = null;
	}
}
