// <copyright file="MockLibraryManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Opal.System
{
	/// <summary>
	/// The mock library manager
	/// TODO: Move into test project
	/// </summary>
	public class MockLibraryManager : ILibraryManager
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='MockLibraryManager'/> class.
		/// </summary>
		public MockLibraryManager()
		{
			_libraries = new Dictionary<Path, MockLibrary>();
			_requests = new List<string>();
		}

		/// <summary>
		/// Register a known library
		/// </summary>
		public MockLibrary RegisterLibrary(Path path)
		{
			var library = new MockLibrary(path.ToString(), _requests);
			_libraries.Add(path, library);

			return library;
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		public IReadOnlyList<string> GetRequests()
		{
			return _requests;
		}

		/// <summary>
		/// Creates a Library for the provided executable path
		/// </summary>
		public ILibrary LoadDynamicLibrary(Path path)
		{
			_requests.Add($"LoadDynamicLibrary: {path}");

			if (_libraries.TryGetValue(path, out var library))
			{
				return library;
			}
			else
			{
				throw new InvalidOperationException($"Cannot load library: {path}");
			}
		}

		private readonly Dictionary<Path, MockLibrary> _libraries;
		private readonly List<string> _requests;
	}
}
