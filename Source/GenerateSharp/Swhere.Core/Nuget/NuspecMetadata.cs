// <copyright file="NuspecMetadata.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Xml.Serialization;

namespace Swhere.Core.Nuget
{
	public class NuspecMetadata
	{
		[XmlElement("id")]
		public string Id { get; set; } = string.Empty;

		[XmlElement("version")]
		public string Version { get; set; } = string.Empty;

		[XmlElement("description")]
		public string Description { get; set; } = string.Empty;

		[XmlElement("authors")]
		public string Authors { get; set; } = string.Empty;

		[XmlElement("releaseNotes")]
		public string? ReleaseNotes { get; set; }

		[XmlElement("copyright")]
		public string? Copyright { get; set; }

		[XmlElement("tags")]
		public string? Tags { get; set; }

		[XmlArray("dependencies")]
		[XmlArrayItem("group", typeof(NuspecDependencyGroup))]
		[XmlArrayItem("dependency", typeof(NuspecDependency))]
		public List<NuspecDependencyBase>? Dependencies { get; set; }
	}
}
