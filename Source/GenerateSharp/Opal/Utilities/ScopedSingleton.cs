// <copyright file="ScopedSingleton.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Diagnostics.CodeAnalysis;

namespace Opal.System;

/// <summary>
/// A scoped process manager registration helper.
/// </summary>
/// <typeparam name="TInterface">The interface type.</typeparam>
[SuppressMessage("Design", "CA1063:Implement IDisposable Correctly", Justification = "RAII")]
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
	[SuppressMessage("Usage", "CA1816:Dispose methods should call SuppressFinalize", Justification = "RAII")]
	public void Dispose()
	{
		LifetimeManager.ClearSingleton<TInterface>();
	}
}