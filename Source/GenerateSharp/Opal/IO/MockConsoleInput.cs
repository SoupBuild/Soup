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
	private readonly IList<string> parentRequests;
	private string readPasswordResponse;
	private string readLineResponse;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockConsoleInput'/> class.
	/// </summary>
	/// <param name="parentRequests">The parent requests.</param>
	public MockConsoleInput(IList<string> parentRequests)
	{
		this.parentRequests = parentRequests;
		this.readPasswordResponse = string.Empty;
		this.readLineResponse = string.Empty;
	}

	/// <summary>
	/// Set the response for a read password request.
	/// </summary>
	/// <param name="response">The response.</param>
	public void SetReadPasswordResponse(string response)
	{
		this.readPasswordResponse = response;
	}

	/// <summary>
	/// Set the response for a read line request.
	/// </summary>
	/// <param name="response">The response.</param>
	public void SetReadLineResponse(string response)
	{
		this.readLineResponse = response;
	}

	/// <summary>
	/// Read a line from the stream.
	/// </summary>
	public virtual string ReadLine()
	{
		this.parentRequests.Add($"ReadLine: {this.readLineResponse}");

		return this.readLineResponse;
	}

	/// <summary>
	/// Read a password from the stream.
	/// </summary>
	public virtual string ReadPassword()
	{
		this.parentRequests.Add($"ReadPassword: {this.readPasswordResponse}");

		return this.readPasswordResponse;
	}
}