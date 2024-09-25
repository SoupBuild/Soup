// <copyright file="MockConsoleInputStream.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Opal.IO;

/// <summary>
/// A mock Console Input Stream.
/// </summary>
public class MockConsoleInput : IConsoleInput
{
	private readonly IList<string> _parentRequests;
	private string _readPasswordResponse;
	private string _readLineResponse;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockConsoleInput'/> class.
	/// </summary>
	/// <param name="parentRequests">The parent requests.</param>
	public MockConsoleInput(IList<string> parentRequests)
	{
		_parentRequests = parentRequests;
		_readPasswordResponse = string.Empty;
		_readLineResponse = string.Empty;
	}

	/// <summary>
	/// Set the response for a read password request.
	/// </summary>
	/// <param name="response">The response.</param>
	public void SetReadPasswordResponse(string response)
	{
		_readPasswordResponse = response;
	}

	/// <summary>
	/// Set the response for a read line request.
	/// </summary>
	/// <param name="response">The response.</param>
	public void SetReadLineResponse(string response)
	{
		_readLineResponse = response;
	}

	/// <summary>
	/// Read a line from the stream.
	/// </summary>
	public virtual string ReadLine()
	{
		_parentRequests.Add($"ReadLine: {_readLineResponse}");

		return _readLineResponse;
	}

	/// <summary>
	/// Read a password from the stream.
	/// </summary>
	public virtual string ReadPassword()
	{
		_parentRequests.Add($"ReadPassword: {_readPasswordResponse}");

		return _readPasswordResponse;
	}
}