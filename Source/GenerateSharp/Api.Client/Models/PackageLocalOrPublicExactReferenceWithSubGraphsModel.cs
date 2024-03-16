// <copyright file="PackageLocalOrPublicExactReferenceWithSubGraphsModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client;

/// <summary>
/// A class representing a local or public reference to an exact package with subgraph closure references.
/// </summary>
public class PackageLocalOrPublicExactReferenceWithSubGraphsModel
{
	/// <summary>
	/// Gets or sets the local package id.
	/// </summary>
	[JsonPropertyName("localId")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public int? LocalId { get; set; }

	/// <summary>
	/// Gets or sets the public package reference.
	/// </summary>
	[JsonPropertyName("public")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public PackagePublicExactReferenceModel? Public { get; set; }

	/// <summary>
	/// Gets or sets the build.
	/// </summary>
	[JsonPropertyName("build")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public required string Build { get; set; }

	/// <summary>
	/// Gets or sets the build.
	/// </summary>
	[JsonPropertyName("tool")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	[Required(AllowEmptyStrings = true)]
	public required string Tool { get; set; }
}