// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using Newtonsoft.Json;
using System;

namespace Soup
{
	public class SemanticVersionJsonConverter : JsonConverter
	{
		public SemanticVersionJsonConverter()
		{
		}

		public override bool CanConvert(Type objectType)
		{
			return objectType == typeof(SemanticVersion);
		}

		public override object ReadJson(
			JsonReader reader,
			Type objectType,
			object existingValue,
			JsonSerializer serializer)
		{
			if (reader.TokenType == JsonToken.Null)
			{
				return null;
			}
			else
			{
				if (reader.TokenType == JsonToken.String)
				{
					try
					{
						SemanticVersion result = SemanticVersion.Parse((string)reader.Value);
						return result;
					}
					catch (Exception ex)
					{
						throw new JsonSerializationException($"Error parsing version string: {reader.Value}", ex);
					}
				}
				else
				{
					throw new JsonSerializationException(
						$"Unexpected token or value when parsing version. Token: {reader.TokenType}, Value: {reader.Value}");
				}
			}
		}

		public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
		{
			if (value == null)
			{
				writer.WriteNull();
			}
			else if (value is SemanticVersion)
			{
				writer.WriteValue(value.ToString());
			}
			else
			{
				throw new JsonSerializationException("Expected SemanticVersion object value");
			}
		}
	}
}