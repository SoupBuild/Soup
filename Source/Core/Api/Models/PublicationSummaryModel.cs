// <copyright company="Soup" file="PublicationSummaryModel.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using Newtonsoft.Json;

	public class PublicationSummaryModel
	{
		[JsonProperty("version")]
		[JsonConverter(typeof(SemanticVersionJsonConverter))]
		public SemanticVersion Version { get; set; }
	}
}
