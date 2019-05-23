// <copyright file="PackageResultModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using System.Collections.Generic;
    using Newtonsoft.Json;

    /// <summary>
    /// A class representing the package result
    /// </summary>
    public class PackageResultModel
    {
        /// <summary>
        /// Gets or sets the name
        /// </summary>
        [JsonProperty("name")]
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets the description
        /// </summary>
        [JsonProperty("description")]
        public string Description { get; set; }

        /// <summary>
        /// Gets or sets the latest published version
        /// </summary>
        [JsonProperty("latest")]
        [JsonConverter(typeof(SemanticVersionJsonConverter))]
        public SemanticVersion Latest { get; set; }

        /// <summary>
        /// Gets or sets the list of publications
        /// </summary>
        [JsonProperty("publications")]
        public IList<PublicationSummaryModel> Publications { get; set; }
    }
}
