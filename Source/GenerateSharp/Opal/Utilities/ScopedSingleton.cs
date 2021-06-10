// <copyright file="ScopedProcessManagerRegister.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal.System
{
	/// <summary>
	/// A scopped process manager registration helper
	/// </summary>
	public class ScopedSingleton<TInterface> : IDisposable where TInterface : notnull
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedSingleton'/> class.
		/// </summary>
		public ScopedSingleton(TInterface listener) 
		{
			LifetimeManager.RegisterSingleton(listener);
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedSingleton'/> class.
		/// </summary>
		public void Dispose()
		{
			LifetimeManager.ClearSingleton<TInterface>();
		}
	};
}
