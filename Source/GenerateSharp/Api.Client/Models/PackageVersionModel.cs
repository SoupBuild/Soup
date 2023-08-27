// <copyright file="PackageVersionModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client
{
	/// <summary>
	/// Package version model.
	/// </summary>
	public class PackageVersionModel
	{
		/// <summary>
		/// Gets or sets the version.
		/// </summary>
		[JsonPropertyName("version")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public SemanticVersion Version { get; set; } = new SemanticVersion();

		/// <summary>
		/// Gets or sets the date published.
		/// </summary>
		[JsonPropertyName("datePublished")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public System.DateTimeOffset DatePublished { get; set; }

		/// <summary>
		/// Gets or sets the total downloads count.
		/// </summary>
		[JsonPropertyName("totalDownloads")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int TotalDownloads { get; set; }
	}
}