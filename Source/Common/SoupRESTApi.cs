// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.IO;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading.Tasks;

namespace Soup
{
	public static class SoupRESTApi
	{
		public static async Task<HttpResponseMessage> PublishAsync(Stream stream)
		{
			using (HttpClient client = new HttpClient())
			{
				var content = new StreamContent(stream);
				content.Headers.ContentType = new MediaTypeHeaderValue("application/zip");
				var response = await client.PutAsync(Constants.SoupRESTPublish, content);

				return response;
			}
		}
	}
}