// <copyright file="MockConsoleManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Opal.IO;

/// <summary>
/// The mock console manager
/// TODO: Move into test project.
/// </summary>
public class MockConsoleManager : IConsoleManager
{
	private readonly List<string> _requests;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockConsoleManager'/> class.
	/// </summary>
	public MockConsoleManager()
	{
		_requests = [];
		MockInputStream = new MockConsoleInput(_requests);
	}

	/// <summary>
	/// Get the requests.
	/// </summary>
	public IReadOnlyList<string> Requests => _requests;

	/// <summary>
	/// Get the mock input stream.
	/// </summary>
	public MockConsoleInput MockInputStream { get; }

	/// <summary>
	/// Get the standard input stream.
	/// </summary>
	public IConsoleInput GetStandardInput()
	{
		_requests.Add("GetStandardInput");

		return MockInputStream;
	}
}