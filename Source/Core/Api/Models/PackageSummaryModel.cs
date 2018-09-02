// <copyright company="Soup" file="PackageSummaryModel.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using Newtonsoft.Json;

	/// <summary>
	/// A container for the package summary result
	/// </summary>
	public class PackageSummaryModel
	{
		[JsonProperty("name")]
		public string Name { get; set; }

		[JsonProperty("description")]
		public string Description { get; set; }
	}
}
