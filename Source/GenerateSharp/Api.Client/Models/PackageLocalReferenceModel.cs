// <copyright file="PackageLocalReferenceModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client;

/// <summary>
/// A class representing the package result.
/// </summary>
public class PackageLocalReferenceModel
{
	/// <summary>
	/// Gets or sets the unique id.
	/// </summary>
	[JsonPropertyName("id")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	public int Id { get; set; }

	/// <summary>
	/// Gets or sets the language reference.
	/// </summary>
	[JsonPropertyName("language")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required]
	public LanguageReferenceModel Language { get; set; } = new LanguageReferenceModel();

	/// <summary>
	/// Gets or sets the list of dependencies.
	/// </summary>
	[JsonPropertyName("dependencies")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public IDictionary<string, ICollection<int>>? Dependencies { get; init; }
}