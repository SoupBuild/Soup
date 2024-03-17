// <copyright file="NuspecDependencyGroup.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Xml;
using System.Xml.Serialization;

namespace Swhere.Core.Nuget;

public class NuspecDependencyGroup : NuspecDependencyBase
{
	[XmlAttribute("targetFramework")]
	public string TargetFramework { get; set; } = string.Empty;

	[XmlElement("dependency")]
	public IList<NuspecDependency> Dependencies { get; init; } = [];

	internal static NuspecDependencyGroup Deserialize(XmlNode node)
	{
		var result = new NuspecDependencyGroup();

		if (node.Attributes is not null)
		{
			for (int i = 0; i < node.Attributes.Count; i++)
			{
				var attribute = node.Attributes[i];
				switch (attribute.Name)
				{
					case "targetFramework":
						result.TargetFramework = attribute.Value ?? string.Empty;
						break;
					default:
						break;
				}
			}
		}

		if (node.HasChildNodes)
		{
			for (int i = 0; i < node.ChildNodes.Count; i++)
			{
				var child = node.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
				switch (child.Name)
				{
					case "dependency":
						var dependency = NuspecDependency.Deserialize(child);
						result.Dependencies.Add(dependency);
						break;
					default:
						break;
				}
			}
		}

		return result;
	}
}