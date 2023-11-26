// <copyright file="NuspecDependency.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Xml;
using System.Xml.Serialization;

namespace Swhere.Core.Nuget;

public class NuspecDependency : NuspecDependencyBase
{
	[XmlAttribute("id")]
	public string Id { get; set; } = string.Empty;

	[XmlAttribute("version")]
	public string Version { get; set; } = string.Empty;

	[XmlAttribute("include")]
	public string? Include { get; set; }

	[XmlAttribute("exclude")]
	public string? Exclude { get; set; }

	internal static NuspecDependency Deserialize(XmlNode node)
	{
		var result = new NuspecDependency();
		if (node.Attributes is not null)
		{
			for (int i = 0; i < node.Attributes.Count; i++)
			{
				var attribute = node.Attributes[i];
				switch (attribute.Name)
				{
					case "id":
						result.Id = attribute.Value ?? string.Empty;
						break;
					case "version":
						result.Version = attribute.Value ?? string.Empty;
						break;
					case "include":
						result.Include = attribute.Value ?? string.Empty;
						break;
					case "exclude":
						result.Exclude = attribute.Value ?? string.Empty;
						break;
				}
			}
		}

		return result;
	}
}