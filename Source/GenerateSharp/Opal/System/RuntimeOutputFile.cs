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
	private readonly FileStream _stream;
	private bool _isDisposed;

	public RuntimeOutputFile(FileStream stream)
	{
		_isDisposed = false;
		_stream = stream;
	}

	/// <summary>
	/// Get the file stream.
	/// </summary>
	public Stream GetOutStream()
	{
		return _stream;
	}

	public void Dispose()
	{
		// Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
		Dispose(disposing: true);
		GC.SuppressFinalize(this);
	}

	private void Dispose(bool disposing)
	{
		if (!_isDisposed)
		{
			if (disposing)
			{
				_stream.Dispose();
			}

			_isDisposed = true;
		}
	}

	Stream IOutputFile.GetOutStream()
	{
		throw new NotImplementedException();
	}
}
