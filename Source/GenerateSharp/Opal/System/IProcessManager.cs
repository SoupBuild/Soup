// <copyright file="IProcessManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal.System
{
	/// <summary>
	/// The process manager interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	public abstract class IProcessManager
	{
		/// <summary>
		/// Gets the current active process manager
		/// </summary>
		public static IProcessManager Current
		{
			get
			{
				if (_current == null)
					throw new InvalidOperationException("No process manager implementation registered.");
				return _current;
			}
		}

		/// <summary>
		/// Register a new active process manager
		/// </summary>
		public static void Register(IProcessManager? value)
		{
			_current = value;
		}

		/// <summary>
		/// Gets the process file name
		/// </summary>
		public abstract Path GetCurrentProcessFileName();

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		public abstract IProcess CreateProcess(
			Path executable,
			string arguments,
			Path workingDirectory);

		private static IProcessManager? _current = null;
	}
}
