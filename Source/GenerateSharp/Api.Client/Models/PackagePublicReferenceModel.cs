// <copyright file="PackagePublicReferenceModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client;

/// <summary>
/// A class representing the package result.
/// </summary>
public class PackagePublicReferenceModel
{
	/// <summary>
	/// Gets or sets the unique id.
	/// </summary>
	[JsonPropertyName("id")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	public int Id { get; set; }

	/// <summary>
	/// Gets or sets the language.
	/// </summary>
	[JsonPropertyName("language")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public string Language { get; set; }

	/// <summary>
	/// Gets or sets the name.
	/// </summary>
	[JsonPropertyName("name")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public string Name { get; set; }

	/// <summary>
	/// Gets or sets the version.
	/// </summary>
	[JsonPropertyName("version")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required]
	public SemanticVersionModel Version { get; set; } = new SemanticVersionModel();
}