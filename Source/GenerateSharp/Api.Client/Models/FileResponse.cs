// <copyright file="FileResponse.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.IO;

namespace Soup.Build.Api.Client;

public sealed class FileResponse : IDisposable
{
	private readonly IDisposable? _client;
	private readonly IDisposable _response;

	public int StatusCode { get; private set; }

	public IReadOnlyDictionary<string, IEnumerable<string>> Headers { get; private set; }

	public Stream Stream { get; private set; }

	public bool IsPartial => StatusCode == 206;

	public FileResponse(
		int statusCode,
		IReadOnlyDictionary<string, IEnumerable<string>> headers,
		Stream stream,
		IDisposable? client,
		IDisposable response)
	{
		StatusCode = statusCode;
		Headers = headers;
		Stream = stream;
		_client = client;
		_response = response;
	}

	public void Dispose()
	{
		Stream.Dispose();
		_response?.Dispose();
		_client?.Dispose();
	}
}