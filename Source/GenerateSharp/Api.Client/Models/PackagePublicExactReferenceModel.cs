// <copyright file="PackagePublicExactReferenceModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client;

/// <summary>
/// A class representing a feed reference to an exact package.
/// </summary>
public class PackagePublicExactReferenceModel
{
	/// <summary>
	/// Gets or sets the language.
	/// </summary>
	[JsonPropertyName("language")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public required string Language { get; set; }

	/// <summary>
	/// Gets or sets the owner.
	/// </summary>
	[JsonPropertyName("owner")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public required string Owner { get; set; }

	/// <summary>
	/// Gets or sets the name.
	/// </summary>
	[JsonPropertyName("name")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public required string Name { get; set; }

	/// <summary>
	/// Gets or sets the version.
	/// </summary>
	[JsonPropertyName("version")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required]
	public required SemanticVersionExactModel Version { get; set; } = new SemanticVersionExactModel();
}