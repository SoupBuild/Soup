// <copyright file="ShimHttpMessageHandler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Soup.Build.PackageManager.UnitTests;

public class ShimHttpMessageHandler : HttpMessageHandler
{
	private readonly IHttpMessageHandler handler;

	public ShimHttpMessageHandler(IHttpMessageHandler handler)
	{
		this.handler = handler;
	}

	public virtual HttpResponseMessage Send(HttpMethod method, Uri requestUri, string headers, string content)
	{
		return this.handler.Send(method, requestUri, headers, content);
	}

	protected override async Task<HttpResponseMessage> SendAsync(
		HttpRequestMessage request, CancellationToken cancellationToken)
	{
		string? content = null;
		if (request.Content != null)
		{
			using var memoryStream = new MemoryStream();
			await request.Content.CopyToAsync(memoryStream, cancellationToken);
			byte[] byteInput = memoryStream.ToArray();
			content = Encoding.UTF8.GetString(byteInput);
		}

		var headers = string.Join(", ", request.Headers.Select(value => $"{{{value.Key}: [{string.Join(", ", value.Value)}]}}"));

		return this.handler.SendAsync(request.Method, request.RequestUri, headers, content);
	}
}