// <copyright file="PackageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client;

/// <summary>
/// A class representing the package result.
/// </summary>
public class PackageModel
{
	/// <summary>
	/// Gets or sets the name.
	/// </summary>
	[JsonPropertyName("name")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public required string Name { get; set; }

	/// <summary>
	/// Gets or sets the owner.
	/// </summary>
	[JsonPropertyName("owner")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public required string Owner { get; set; }

	/// <summary>
	/// Gets or sets the description.
	/// </summary>
	[JsonPropertyName("description")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public string? Description { get; set; }

	/// <summary>
	/// Gets or sets the latest published version.
	/// </summary>
	[JsonPropertyName("latest")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public SemanticVersion? Latest { get; set; }

	/// <summary>
	/// Gets or sets the list of versions.
	/// </summary>
	[JsonPropertyName("versions")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public ICollection<PackageVersionModel>? Versions { get; init; }
}