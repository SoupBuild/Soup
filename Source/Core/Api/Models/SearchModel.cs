// <copyright company="Soup" file="SearchModel.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using System.Collections.Generic;
	using Newtonsoft.Json;

	/// <summary>
	/// A container for the search result
	/// </summary>
	public class SearchModel
	{
		[JsonProperty("total")]
		public int Total { get; set; }

		[JsonProperty("packages")]
		public IList<PackageSummaryModel> Packages { get; set; }
	}
}
