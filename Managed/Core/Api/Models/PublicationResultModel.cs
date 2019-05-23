// <copyright file="PublicationResultModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using System;
    using Newtonsoft.Json;

    /// <summary>
    /// Publication result model
    /// </summary>
    public class PublicationResultModel
    {
        /// <summary>
        /// Gets or sets the version
        /// </summary>
        [JsonProperty("version")]
        [JsonConverter(typeof(SemanticVersionJsonConverter))]
        public SemanticVersion Version { get; set; }

        /// <summary>
        /// Gets or sets the date published
        /// </summary>
        [JsonProperty("datePublished")]
        public DateTime DatePublished { get; set; }

        /// <summary>
        /// Gets or sets the total downloads count
        /// </summary>
        [JsonProperty("totalDownloads")]
        public int TotalDownloads { get; set; }
    }
}
