// <copyright file="PublicationSummaryModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using Newtonsoft.Json;

    /// <summary>
    /// The publication summary model
    /// </summary>
    public class PublicationSummaryModel
    {
        /// <summary>
        /// Gets or sets the version
        /// </summary>
        [JsonProperty("version")]
        [JsonConverter(typeof(SemanticVersionJsonConverter))]
        public SemanticVersion Version { get; set; }
    }
}
