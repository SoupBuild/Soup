// <copyright company="Soup" file="PackageGetResult.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using System.Collections.Generic;

	/// <summary>
	/// A class representing the package result
	/// </summary>
	public class PackageGetResult
	{
		public string Name { get; set; }

		public string Description { get; set; }

		public SemanticVersion Latest { get; set; }

		public IList<PublicationSummary> Publications { get; set; }
	}
}
