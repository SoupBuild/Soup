// <copyright file="RuntimeInputFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System;

/// <summary>
/// The standard library input file implementation.
/// </summary>
internal sealed class RuntimeInputFile : IInputFile
{
	private readonly FileStream _stream;
	private bool _isDisposed;

	public RuntimeInputFile(FileStream stream)
	{
		isDisposed = false;
		this.stream = stream;
	}

	/// <summary>
	/// Gets the file stream.
	/// </summary>
	public Stream GetInStream()
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
}