// <copyright file="ShimHttpMessageHandler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace Soup.Build.PackageManager.UnitTests
{
	public class ShimHttpMessageHandler : HttpMessageHandler
	{
		public virtual HttpResponseMessage Send(HttpMethod method, Uri requestUri, string headers, string content)
		{
			throw new NotImplementedException("Use Moq to overrite this method");
		}

		protected override async Task<HttpResponseMessage> SendAsync(HttpRequestMessage request, System.Threading.CancellationToken cancellationToken)
		{
			string content = null;
			if (request.Content != null)
			{
				using var memoryStream = new MemoryStream();
				await request.Content.CopyToAsync(memoryStream);
				byte[] byteInput = memoryStream.ToArray();
				content = Encoding.UTF8.GetString(byteInput);
			}

			return Send(
				request.Method,
				request.RequestUri,
				string.Join(", ", request.Headers.Select(value => $"{{{value.Key}: [{string.Join(", ", value.Value)}]}}")),
				content);
		}
	}
}
