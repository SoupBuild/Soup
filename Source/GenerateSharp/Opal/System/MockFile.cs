// <copyright file="MockFileSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.IO;

namespace Opal.System
{
	public class MockFile
	{
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

		public MemoryStream Content;
		public int LastWriteTime;
	}
	
	public class MockOutputFile : IOutputFile
	{
		public MockOutputFile(MockFile file)
		{
			_file = file;
		}

		/// <summary>
		/// Gets the file stream
		/// </summary>
		public Stream GetOutStream()
		{
			return _file.Content;
		}

		/// <summary>
		/// Close the file stream
		/// </summary>
		public void Close()
		{
			// Ignore for now
		}
	
		private MockFile _file;
	}
	
	public class MockInputFile : IInputFile
	{
		public MockInputFile(MockFile file)
		{
			_file = file;
		}

		/// <summary>
		/// Gets the file stream
		/// </summary>
		public Stream GetInStream()
		{
			return _file.Content;
		}

		/// <summary>
		/// Close the file stream
		/// </summary>
		public void Close()
		{
			// Ignore for now
		}

		private MockFile _file;
	}
}
