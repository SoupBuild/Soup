// <copyright file="TestUtils.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System.IO;
    using Newtonsoft.Json;

    /// <summary>
    /// Test utilities
    /// </summary>
    public static class TestUtils
    {
        /// <summary>
        /// Serialize json
        /// </summary>
        public static string JsonSerialize(object value)
        {
            var serializer = new JsonSerializer();
            using (var writer = new StringWriter())
            {
                serializer.Serialize(writer, value);
                return writer.ToString();
            }
        }

        /// <summary>
        /// Deserialize json
        /// </summary>
        /// <typeparam name="T">The expected object type</typeparam>
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
