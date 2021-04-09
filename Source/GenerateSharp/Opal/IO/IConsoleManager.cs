// <copyright file="IConsoleManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal.IO
{
	/// <summary>
	/// The input manager interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	public abstract class IConsoleManager
	{
		/// <summary>
		/// Gets the current active manager
		/// </summary>
		public static IConsoleManager Current()
		{
			if (_current == null)
				throw new InvalidOperationException("No console manager implementation registered.");
			return _current;
		}

		/// <summary>
		/// Register a new active input manager
		/// </summary>
		public static void Register(IConsoleManager? manager)
		{
			_current = manager;
		}

		/// <summary>
		/// Get the standard input stream
		/// </summary>
		public abstract IConsoleInputStream GetStandardInput();

		private static IConsoleManager? _current = null;
	}
}
