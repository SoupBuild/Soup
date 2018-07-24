// <copyright company="Soup" file="SearchModel.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using System.Collections.Generic;

	/// <summary>
	/// A container for the search result
	/// </summary>
	public class SearchModel
	{
		public int Total { get; set; }

		public IList<PackageSummaryModel> Packages { get; set; }
	}
}
