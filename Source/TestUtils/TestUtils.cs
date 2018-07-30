// <copyright company="Soup" file="TestUtils.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using Newtonsoft.Json;
	using System.IO;

	public static class TestUtils
	{
		public static string JsonSerialize(object value)
		{
			var serializer = new JsonSerializer();
			using (var writer = new StringWriter())
			{
				serializer.Serialize(writer, value);
				return writer.ToString();
			}
		}

		public static T JsonDeserialize<T>(string json)
		{
			var serializer = new JsonSerializer();
			using (var textReader = new StringReader(json))
			using (var reader = new JsonTextReader(textReader))
			{
				return serializer.Deserialize<T>(reader);
			}
		}
	}
}
