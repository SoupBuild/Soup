// <copyright file="PackageCreateOrUpdateModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client
{
	/// <summary>
	/// A container for the package creation or update request.
	/// </summary>
	public class PackageCreateOrUpdateModel
	{
		/// <summary>
		/// Gets or sets the description.
		/// </summary>
		[JsonPropertyName("description")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		[StringLength(1024)]
		public string Description { get; set; }
	}
}