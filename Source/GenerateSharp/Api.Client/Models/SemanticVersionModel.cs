// <copyright file="SemanticVersionModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client
{
	/// <summary>
	/// A class representing the semantic version model
	/// </summary>
	public class SemanticVersionModel
	{
		/// <summary>
		/// Gets or sets the version major.
		/// </summary>
		[JsonPropertyName("major")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		public int Major { get; set; }

		/// <summary>
		/// Gets or sets the version minor.
		/// </summary>
		[JsonPropertyName("minor")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? Minor { get; set; }

		/// <summary>
		/// Gets or sets the version patch.
		/// </summary>
		[JsonPropertyName("patch")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? Patch { get; set; }
	}
}