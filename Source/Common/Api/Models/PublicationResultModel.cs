// <copyright company="Soup" file="PublicationResultModel.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using System;

	public class PublicationResultModel
	{
		public SemanticVersion Version { get; set; }

		public DateTime DatePublished { get; set; }

		public int TotalDownloads { get; set; }
	}
}
