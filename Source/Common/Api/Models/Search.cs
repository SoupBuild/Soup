// <copyright company="Soup" file="Search.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using System.Collections.Generic;

	/// <summary>
	/// A container for the search result
	/// </summary>
	public class Search
	{
		public int Total { get; set; }

		public IList<PackageSummary> Packages { get; set; }
	}
}
