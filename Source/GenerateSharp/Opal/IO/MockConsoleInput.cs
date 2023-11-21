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
	private List<string> _parentRequests;
	private string _readPasswordResponse;
	private string _readLineResponse;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockConsoleInput'/> class.
	/// </summary>
	/// <param name="parentRequests">The parent requests.</param>
	public MockConsoleInput(List<string> parentRequests)
	{
		this._parentRequests = parentRequests;
		this._readPasswordResponse = string.Empty;
		this._readLineResponse = string.Empty;
	}

	/// <summary>
	/// Set the response for a read password request.
	/// </summary>
	/// <param name="response">The response.</param>
	public void SetReadPasswordResponse(string response)
	{
		this._readPasswordResponse = response;
	}

	/// <summary>
	/// Set the response for a read line request.
	/// </summary>
	/// <param name="response">The response.</param>
	public void SetReadLineResponse(string response)
	{
		this._readLineResponse = response;
	}

	/// <summary>
	/// Read a line from the stream.
	/// </summary>
	public virtual string ReadLine()
	{
		this._parentRequests.Add($"ReadLine: {this._readLineResponse}");

		return this._readLineResponse;
	}

	/// <summary>
	/// Read a password from the stream.
	/// </summary>
	public virtual string ReadPassword()
	{
		this._parentRequests.Add($"ReadPassword: {this._readPasswordResponse}");

		return this._readPasswordResponse;
	}
}