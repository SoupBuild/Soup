// <copyright file="RuntimeOutputFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.IO;

namespace Opal.System;

/// <summary>
/// The standard library output file implementation.
/// </summary>
internal sealed class RuntimeOutputFile : IOutputFile
{
	private readonly FileStream _stream;
	private bool _isDisposed;

	public RuntimeOutputFile(FileStream stream)
	{
		isDisposed = false;
		this.stream = stream;
	}

	/// <summary>
	/// Get the file stream.
	/// </summary>
	public Stream GetOutStream()
	{
		return stream;
	}

	public void Dispose()
	{
		// Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
		Dispose(disposing: true);
		GC.SuppressFinalize(this);
	}

	private void Dispose(bool disposing)
	{
		if (!isDisposed)
		{
			if (disposing)
			{
				stream.Dispose();
			}

			isDisposed = true;
		}
	}

	Stream IOutputFile.GetOutStream()
	{
		throw new global::System.NotImplementedException();
	}
}
