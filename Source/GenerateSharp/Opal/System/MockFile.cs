// <copyright file="MockFile.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.IO;

namespace Opal.System;

public class MockFile
{
	public MemoryStream Content { get; set; }

	public int LastWriteTime { get; set; }

	public MockFile(MemoryStream content, int lastWriteTime)
	{
		Content = content;
		LastWriteTime = lastWriteTime;
	}

	public MockFile(MemoryStream content)
	{
		Content = content;
		LastWriteTime = 0;
	}

	public MockFile(int lastWriteTime)
	{
		Content = new MemoryStream();
		LastWriteTime = lastWriteTime;
	}

	public MockFile()
	{
		Content = new MemoryStream();
		LastWriteTime = 0;
	}
}

public class MockOutputFile : IOutputFile
{
	private readonly MockFile file;
	private bool isDisposed;

	public MockOutputFile(MockFile file)
	{
		isDisposed = false;
		this.file = file;
	}

	/// <summary>
	/// Gets the file stream.
	/// </summary>
	public Stream GetOutStream()
	{
		return file.Content;
	}

	public void Dispose()
	{
		// Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
		Dispose(disposing: true);
		global::System.GC.SuppressFinalize(this);
	}

	protected virtual void Dispose(bool disposing)
	{
		if (!isDisposed)
		{
			isDisposed = true;
		}
	}

	Stream IOutputFile.GetOutStream()
	{
		throw new global::System.NotImplementedException();
	}
}

public class MockInputFile : IInputFile
{
	private readonly MockFile _file;
	private bool isDisposed;

	public MockInputFile(MockFile file)
	{
		isDisposed = false;
		_file = file;
	}

	/// <summary>
	/// Gets the file stream.
	/// </summary>
	public Stream GetInStream()
	{
		_ = _file.Content.Seek(0, SeekOrigin.Begin);
		return _file.Content;
	}

	public void Dispose()
	{
		// Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
		Dispose(disposing: true);
		global::System.GC.SuppressFinalize(this);
	}

	protected virtual void Dispose(bool disposing)
	{
		if (!isDisposed)
		{
			isDisposed = true;
		}
	}

	Stream IInputFile.GetInStream()
	{
		throw new global::System.NotImplementedException();
	}

	Stream IInputFile.GetInStream()
	{
		throw new global::System.NotImplementedException();
	}

	Stream IInputFile.GetInStream()
	{
		throw new global::System.NotImplementedException();
	}
}