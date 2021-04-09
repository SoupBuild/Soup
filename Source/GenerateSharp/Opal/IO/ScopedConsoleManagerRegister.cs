// <copyright file="ScopedConsoleManagerRegister.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal.IO
{
	/// <summary>
	/// A scopped network manager registration helper
	/// </summary>
	public class ScopedConsoleManagerRegister : IDisposable
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedConsoleManagerRegister'/> class.
		/// </summary>
		public ScopedConsoleManagerRegister(IConsoleManager manager)
		{
			IConsoleManager.Register(manager);
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedConsoleManagerRegister'/> class.
		/// </summary>
		public void Dispose()
		{
			IConsoleManager.Register(null);
		}
    }
}
