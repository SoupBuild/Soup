// <copyright file="ScopedTraceListenerRegister.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal
{
	/// <summary>
	/// A scopped trace listener registration helper.
	/// </summary>
	public class ScopedTraceListenerRegister : IDisposable
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedTraceListenerRegister'/> class.
		/// </summary>
		public ScopedTraceListenerRegister(TraceListener listener)
		{
			Log.RegisterListener(listener);
		}

		public void Dispose()
		{
			Log.RegisterListener(null);
		}
	}
}
