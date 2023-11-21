// <copyright file="NuspecMetadata.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;

namespace Swhere.Core.Nuget;

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

	internal static NuspecMetadata Deserialize(XmlNode node)
	{
		var result = new NuspecMetadata();
		if (node.HasChildNodes)
		{
			for (int i = 0; i < node.ChildNodes.Count; i++)
			{
				var child = node.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
				switch (child.Name)
				{
					case "id":
						result.Id = child.InnerText ?? string.Empty;
						break;
					case "version":
						result.Version = child.InnerText ?? string.Empty;
						break;
					case "description":
						result.Description = child.InnerText ?? string.Empty;
						break;
					case "authors":
						result.Authors = child.InnerText ?? string.Empty;
						break;
					case "releaseNotes":
						result.ReleaseNotes = child.InnerText ?? string.Empty;
						break;
					case "copyright":
						result.Copyright = child.InnerText ?? string.Empty;
						break;
					case "tags":
						result.Tags = child.InnerText ?? string.Empty;
						break;
					case "dependencies":
						result.Dependencies = DeserializeDependencies(child);
						break;
				}
			}
		}

		return result;
	}

	private static List<NuspecDependencyBase> DeserializeDependencies(XmlNode node)
	{
		var result = new List<NuspecDependencyBase>();
		if (node.HasChildNodes)
		{
			for (int i = 0; i < node.ChildNodes.Count; i++)
			{
				var child = node.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
				switch (child.Name)
				{
					case "group":
						var group = NuspecDependencyGroup.Deserialize(child);
						result.Add(group);
						break;
				}
			}
		}

		return result;
	}
}