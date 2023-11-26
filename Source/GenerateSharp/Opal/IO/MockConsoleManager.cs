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
	private List<string> _requests;
	private MockConsoleInput _standardInput;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockConsoleManager'/> class.
	/// </summary>
	public MockConsoleManager()
	{
		this._requests = new List<string>();
		this._standardInput = new MockConsoleInput(this._requests);
	}

	/// <summary>
	/// Get the requests.
	/// </summary>
	public IReadOnlyList<string> Requests => this._requests;

	/// <summary>
	/// Get the mock input stream.
	/// </summary>
	public MockConsoleInput MockInputStream => this._standardInput;

	/// <summary>
	/// Get the standard input stream.
	/// </summary>
	public IConsoleInput GetStandardInput()
	{
		this._requests.Add("GetStandardInput");

		return this._standardInput;
	}
}