// <copyright file="GenerateClosureResultModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client;

/// <summary>
/// A container for the generate closure result.
/// </summary>
public class GenerateClosureResultModel
{
	/// <summary>
	/// Gets or sets the result
	/// </summary>
	[JsonPropertyName("result")]
	[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
	public GenerateClosureResult Result { get; set; }

	/// <summary>
	/// Gets or sets the message
	/// </summary>
	[JsonPropertyName("message")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public string? Message { get; set; }

	/// <summary>
	/// Gets or sets the runtime closure.
	/// </summary>
	[JsonPropertyName("runtimeClosure")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public ICollection<PackageLocalOrPublicExactReferenceWithSubGraphsModel>? RuntimeClosure { get; init; }

	/// <summary>
	/// Gets or sets the collection of build closures.
	/// </summary>
	[JsonPropertyName("buildClosures")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public IDictionary<string, ICollection<PackageLocalOrPublicExactReferenceModel>>? BuildClosures { get; init; }

	/// <summary>
	/// Gets or sets the collection of tool closures.
	/// </summary>
	[JsonPropertyName("toolClosures")]
	[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
	public IDictionary<string, ICollection<PackageLocalOrPublicExactReferenceModel>>? ToolClosures { get; init; }
}