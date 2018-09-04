// <copyright file="PackageReferenceJsonConverter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System;
    using Newtonsoft.Json;

    /// <summary>
    /// The Json converter for <see cref="PackageReference"/>
    /// </summary>
    internal class PackageReferenceJsonConverter : JsonConverter
    {
        /// <summary>
        /// Check if the converter can handle the provided type
        /// </summary>
        public override bool CanConvert(Type objectType)
        {
            return objectType == typeof(PackageReference);
        }

        /// <summary>
        /// Read json
        /// </summary>
        public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
        {
            if (reader.TokenType != JsonToken.String)
            {
                return null;
            }

            var result = existingValue as PackageReference ?? new PackageReference();
            var value = reader.Value as string;
            if (PackageReference.TryParse(value, out PackageReference newValue))
            {
                result = newValue;
            }

            return result;
        }

        /// <summary>
        /// Write json
        /// </summary>
        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {
            var packageReference = (PackageReference)value;
            serializer.Serialize(writer, packageReference.ToString());
        }
    }
}
