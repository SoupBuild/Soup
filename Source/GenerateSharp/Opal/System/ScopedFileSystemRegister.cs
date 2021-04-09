// <copyright file="ScopedFileSystemRegister.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal.System
{
	/// <summary>
	/// A scopped file system registration helper
	/// </summary>
	public class ScopedFileSystemRegister : IDisposable
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedFileSystemRegister'/> class.
		/// </summary>
		public ScopedFileSystemRegister(IFileSystem listener)
		{
			IFileSystem.Register(listener);
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedFileSystemRegister'/> class.
		/// </summary>
		public void Dispose()
		{
			IFileSystem.Register(null);
		}
	};
}
