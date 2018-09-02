// <copyright company="Soup" file="PackageReferenceJsonConverter.cs">
//   Copyright (c) Soup.  All rights reserved.
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
		public override bool CanConvert(Type objectType)
		{
			return objectType == typeof(PackageReference);
		}

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

		public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
		{
			var packageReference = (PackageReference)value;
			serializer.Serialize(writer, packageReference.ToString());
		}
	}
}
