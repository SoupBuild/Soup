// <copyright company="Soup" file="PublicationResultModel.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using Newtonsoft.Json;
	using System;

	public class PublicationResultModel
	{
		[JsonProperty("version")]
		[JsonConverter(typeof(SemanticVersionJsonConverter))]
		public SemanticVersion Version { get; set; }

		[JsonProperty("datePublished")]
		public DateTime DatePublished { get; set; }

		[JsonProperty("totalDownloads")]
		public int TotalDownloads { get; set; }
	}
}
