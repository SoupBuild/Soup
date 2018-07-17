// <copyright company="Soup" file="Package.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Collections.Generic;

	/// <summary>
	/// A class representing the package result
	/// </summary>
	public class Package
	{
		public string Name { get; set; }

		public string Description { get; set; }

		public string Latest { get; set; }

		public IList<Publication> Publications { get; set; }
	}
}
