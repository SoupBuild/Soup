// <copyright file="SearchPackagesModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client
{
	/// <summary>
	/// A container for the search result.
	/// </summary>
	public class SearchPackagesModel
	{
		/// <summary>
		/// Gets or sets the total packages that match the query.
		/// </summary>
		[JsonPropertyName("total")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Total { get; set; }

		/// <summary>
		/// Gets or sets the start offset for this set of packages.
		/// </summary>
		[JsonPropertyName("offset")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Offset { get; set; }

		/// <summary>
		/// Gets or sets the list of packages.
		/// </summary>
		[JsonPropertyName("packages")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required]
		public ICollection<PackageSummaryModel> Packages { get; set; } = new Collection<PackageSummaryModel>();
	}
}