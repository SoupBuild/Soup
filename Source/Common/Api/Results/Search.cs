// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Api.Results
{
	public class Search
	{
		public int Total { get; set; }

		public IList<PackageSummary> Packages { get; set; }
	}
}
