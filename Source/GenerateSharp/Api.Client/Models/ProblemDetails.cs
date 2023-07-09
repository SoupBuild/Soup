// <copyright file="ProblemDetails.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text.Json.Serialization;

namespace Soup.Build.Api.Client
{
	public class ProblemDetails
	{
		[JsonPropertyName("type")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Type { get; set; }

		[JsonPropertyName("title")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Title { get; set; }

		[JsonPropertyName("status")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public int? Status { get; set; }

		[JsonPropertyName("detail")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Detail { get; set; }

		[JsonPropertyName("instance")]
		[JsonIgnore(Condition = JsonIgnoreCondition.WhenWritingDefault)]
		public string Instance { get; set; }

		private IDictionary<string, object> _additionalProperties;

		[JsonExtensionData]
		public IDictionary<string, object> AdditionalProperties
		{
			get { return _additionalProperties ?? (_additionalProperties = new Dictionary<string, object>()); }
			set { _additionalProperties = value; }
		}

	}
}