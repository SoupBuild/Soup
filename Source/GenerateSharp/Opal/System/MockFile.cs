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
		private MockFile _file;
		private bool isDisposed;

		public MockOutputFile(MockFile file)
		{
			this.isDisposed = false;
			_file = file;
		}

		/// <summary>
		/// Gets the file stream
		/// </summary>
		public Stream GetOutStream()
		{
			return _file.Content;
		}
	
        protected virtual void Dispose(bool disposing)
        {
            if (!isDisposed)
            {
				isDisposed = true;
            }
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            global::System.GC.SuppressFinalize(this);
        }
    }
	
	public class MockInputFile : IInputFile
	{
		private MockFile _file;
		private bool isDisposed;

		public MockInputFile(MockFile file)
		{
			this.isDisposed = false;
			_file = file;
		}

		/// <summary>
		/// Gets the file stream
		/// </summary>
		public Stream GetInStream()
		{
			return _file.Content;
		}

		protected virtual void Dispose(bool disposing)
		{
			if (!isDisposed)
			{
				isDisposed = true;
			}
		}

		public void Dispose()
		{
			// Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
			Dispose(disposing: true);
			global::System.GC.SuppressFinalize(this);
		}
	}
}
