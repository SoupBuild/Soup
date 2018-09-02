// <copyright company="Soup" file="PackageGetResult.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using Newtonsoft.Json;
	using System.Collections.Generic;

	/// <summary>
	/// A class representing the package result
	/// </summary>
	public class PackageResultModel
	{
		[JsonProperty("name")]
		public string Name { get; set; }

		[JsonProperty("description")]
		public string Description { get; set; }

		[JsonProperty("latest")]
		[JsonConverter(typeof(SemanticVersionJsonConverter))]
		public SemanticVersion Latest { get; set; }

		[JsonProperty("publications")]
		public IList<PublicationSummaryModel> Publications { get; set; }
	}
}
