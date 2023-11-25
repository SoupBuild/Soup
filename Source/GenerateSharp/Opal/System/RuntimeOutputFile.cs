// <copyright file="RuntimeOutputFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.IO;

namespace Opal.System;

/// <summary>
/// The standard library output file implementation.
/// </summary>
internal sealed class RuntimeOutputFile : IOutputFile
{
	private readonly FileStream stream;
	private bool isDisposed;

	public RuntimeOutputFile(FileStream stream)
	{
		this.isDisposed = false;
		this.stream = stream;
	}

	/// <summary>
	/// Get the file stream.
	/// </summary>
	public Stream GetOutStream()
	{
		return this.stream;
	}

	public void Dispose()
	{
		// Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
		this.Dispose(disposing: true);
		GC.SuppressFinalize(this);
	}

	private void Dispose(bool disposing)
	{
		if (!this.isDisposed)
		{
			if (disposing)
			{
				this.stream.Dispose();
			}

			this.isDisposed = true;
		}
	}
}
