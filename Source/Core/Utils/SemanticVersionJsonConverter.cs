// <copyright file="SemanticVersionJsonConverter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System;
    using Newtonsoft.Json;

    /// <summary>
    /// The Json Converter for the <see cref="SemanticVersion" /> class
    /// </summary>
    public class SemanticVersionJsonConverter : JsonConverter
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="SemanticVersionJsonConverter"/> class.
        /// </summary>
        public SemanticVersionJsonConverter()
        {
        }

        /// <summary>
        /// Check if this converter can handle the provided type
        /// </summary>
        public override bool CanConvert(Type objectType)
        {
            return objectType == typeof(SemanticVersion);
        }

        /// <summary>
        /// Read the value from json
        /// </summary>
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

        /// <summary>
        /// Write the json
        /// </summary>
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