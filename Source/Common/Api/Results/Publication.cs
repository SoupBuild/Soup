// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;

namespace Soup.Api.Results
{
	public class Publication
	{
		public string Version { get; set; }
		public DateTime DatePublished { get; set; }
		public int TotalDownloads { get; set; }
	}
}