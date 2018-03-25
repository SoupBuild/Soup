// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Api.Results
{
	public class Package
	{
		public string Name { get; set; }
		public string Latest { get; set; }
		public IList<Publication> Publications { get; set; }
	}
}