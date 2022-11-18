// <copyright file="ScopedSingleton.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System
{
	using global::System;

	/// <summary>
	/// A scopped process manager registration helper.
	/// </summary>
	/// <typeparam name="TInterface">The interface type.</typeparam>
	public class ScopedSingleton<TInterface> : IDisposable
		where TInterface : notnull
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedSingleton{TInterface}'/> class.
		/// </summary>
		/// <param name="listener">The listener.</param>
		public ScopedSingleton(TInterface listener)
		{
			LifetimeManager.RegisterSingleton(listener);
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedSingleton{TInterface}'/> class.
		/// </summary>
		public void Dispose()
		{
			LifetimeManager.ClearSingleton<TInterface>();
		}
	}
}
