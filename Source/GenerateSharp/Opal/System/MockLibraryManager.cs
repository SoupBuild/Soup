// <copyright file="MockLibraryManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Opal.System;

/// <summary>
/// The mock library manager
/// TODO: Move into test project.
/// </summary>
public class MockLibraryManager : ILibraryManager
{
	private readonly Dictionary<Path, MockLibrary> _libraries;
	private readonly List<string> _requests;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockLibraryManager'/> class.
	/// </summary>
	public MockLibraryManager()
	{
		this._libraries = new Dictionary<Path, MockLibrary>();
		this._requests = new List<string>();
	}

	/// <summary>
	/// Register a known library.
	/// </summary>
	/// <param name="path">The path.</param>
	public MockLibrary RegisterLibrary(Path path)
	{
		var library = new MockLibrary(path.ToString(), this._requests);
		this._libraries.Add(path, library);

		return library;
	}

	/// <summary>
	/// Get the load requests.
	/// </summary>
	public IReadOnlyList<string> Requests => this._requests;

	/// <summary>
	/// Creates a Library for the provided executable path.
	/// </summary>
	/// <param name="library">The path.</param>
	public ILibrary LoadDynamicLibrary(Path library)
	{
		this._requests.Add($"LoadDynamicLibrary: {library}");

		if (this._libraries.TryGetValue(library, out var libraryValue))
		{
			return libraryValue;
		}
		else
		{
			throw new InvalidOperationException($"Cannot load library: {library}");
		}
	}
}