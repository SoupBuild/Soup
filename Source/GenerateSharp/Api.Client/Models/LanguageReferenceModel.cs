// <copyright file="LanguageReferenceModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client;

/// <summary>
/// A class representing the language version reference.
/// </summary>
public class LanguageReferenceModel
{
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