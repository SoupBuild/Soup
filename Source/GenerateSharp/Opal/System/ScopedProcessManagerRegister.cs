// <copyright file="ScopedProcessManagerRegister.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal.System
{
	/// <summary>
	/// A scopped process manager registration helper
	/// </summary>
	public class ScopedProcessManagerRegister : IDisposable
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedProcessManagerRegister'/> class.
		/// </summary>
		public ScopedProcessManagerRegister(IProcessManager listener)
		{
			IProcessManager.Register(listener);
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedProcessManagerRegister'/> class.
		/// </summary>
		public void Dispose()
		{
			IProcessManager.Register(null);
		}
	};
}
