// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using Newtonsoft.Json;
using System;

namespace Soup
{
	internal class PackageReferenceJsonConverter : JsonConverter
	{
		public override bool CanConvert(Type objectType)
		{
			return objectType == typeof(PackageReference);
		}

		public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
		{
			if (reader.TokenType != JsonToken.String)
				return null;

			var result = (existingValue as PackageReference ?? new PackageReference());
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
