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
	public IList<NuspecDependencyBase>? Dependencies { get; init; }

	internal static NuspecMetadata Deserialize(XmlNode node)
	{
		string id = string.Empty;
		string version = string.Empty;
		string description = string.Empty;
		string authors = string.Empty;
		string? releaseNotes = null;
		string? copyright = null;
		string? tags = null;
		IList<NuspecDependencyBase>? dependencies = null;
		if (node.HasChildNodes)
		{
			for (int i = 0; i < node.ChildNodes.Count; i++)
			{
				var child = node.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
				switch (child.Name)
				{
					case "id":
						id = child.InnerText ?? string.Empty;
						break;
					case "version":
						version = child.InnerText ?? string.Empty;
						break;
					case "description":
						description = child.InnerText ?? string.Empty;
						break;
					case "authors":
						authors = child.InnerText ?? string.Empty;
						break;
					case "releaseNotes":
						releaseNotes = child.InnerText ?? string.Empty;
						break;
					case "copyright":
						copyright = child.InnerText ?? string.Empty;
						break;
					case "tags":
						tags = child.InnerText ?? string.Empty;
						break;
					case "dependencies":
						dependencies = DeserializeDependencies(child);
						break;
				}
			}
		}

		return new NuspecMetadata()
		{
			Id = id,
			Version = version,
			Description = description,
			Authors = authors,
			ReleaseNotes = releaseNotes,
			Copyright = copyright,
			Tags = tags,
			Dependencies = dependencies,
		};
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