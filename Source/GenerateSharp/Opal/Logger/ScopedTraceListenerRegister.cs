// <copyright file="ScopedTraceListenerRegister.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Diagnostics.CodeAnalysis;

namespace Opal;

/// <summary>
/// A scoped trace listener registration helper.
/// </summary>
[SuppressMessage("Design", "CA1063:Implement IDisposable Correctly", Justification = "RAII disposable")]
public class ScopedTraceListenerRegister : IDisposable
{
	/// <summary>
	/// Initializes a new instance of the <see cref='ScopedTraceListenerRegister'/> class.
	/// </summary>
	public ScopedTraceListenerRegister(TraceListener listener)
	{
		Log.RegisterListener(listener);
	}

	[SuppressMessage("Usage", "CA1816:Dispose methods should call SuppressFinalize", Justification = "RAII disposable")]
	public void Dispose()
	{
		Log.RegisterListener(null);
	}
}
