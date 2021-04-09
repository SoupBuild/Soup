// <copyright file="ScopedLibraryManagerRegister.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal.System
{
	/// <summary>
	/// A scopped library manager registration helper
	/// </summary>
	public class ScopedLibraryManagerRegister : IDisposable
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedLibraryManagerRegister'/> class.
		/// </summary>
		public ScopedLibraryManagerRegister(ILibraryManager listener)
		{
			ILibraryManager.Register(listener);
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedLibraryManagerRegister'/> class.
		/// </summary>
		public void Dispose()
		{
			ILibraryManager.Register(null);
		}
	};
}
