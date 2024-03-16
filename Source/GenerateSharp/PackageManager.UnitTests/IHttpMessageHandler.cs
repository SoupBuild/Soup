// <copyright file="ShimHttpMessageHandler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.PackageManager.UnitTests;

public interface IHttpMessageHandler
{
	public HttpResponseMessage Send(HttpMethod method, Uri requestUri, string headers, string? content);
	public HttpResponseMessage SendAsync(HttpMethod method, Uri? requestUri, string headers, string? content);
}
