// <copyright file="NuspecDependency.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Xml.Serialization;

namespace Swhere.Core.Nuget
{
	public class NuspecDependency : NuspecDependencyBase
	{
		[XmlAttribute("id")]
		public string Id { get; set; } = string.Empty;

		[XmlAttribute("version")]
		public string Version { get; set; } = string.Empty;

		[XmlAttribute("include")]
		public string? Description { get; set; }

		[XmlAttribute("exclude")]
		public string? Authors { get; set; }
	}
}
