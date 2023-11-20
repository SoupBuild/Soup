// <copyright file="LanguageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client
{
	/// <summary>
	/// A class representing the package result.
	/// </summary>
	public class LanguageModel
	{
		/// <summary>
		/// Gets or sets the name.
		/// </summary>
		[JsonPropertyName("name")]
		[JsonIgnore(Condition = JsonIgnoreCondition.Never)]
		[Required(AllowEmptyStrings = true)]
		public string Name { get; set; }

		/// <summary>
		/// Gets or sets the description.
		/// </summary>
		[JsonPropertyName("description")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Description { get; set; }
	}
}