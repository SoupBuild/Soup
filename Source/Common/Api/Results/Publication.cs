// <copyright company="Soup" file="Publication.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;

	public class Publication
	{
		public string Version { get; set; }

		public DateTime DatePublished { get; set; }

		public int TotalDownloads { get; set; }
	}
}
