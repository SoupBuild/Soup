// <copyright file="FrameworkFileList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Xml;
using System.Xml.Serialization;

namespace Soup.Build.Discover;

public class FrameworkFile
{
	[XmlAttribute("Type")]
	public string Type { get; set; } = string.Empty;

	[XmlAttribute("Path")]
	public Path Path { get; set; } = new Path();

	[XmlAttribute("Language")]
	public string? Language { get; set; }

	[XmlAttribute("AssemblyName")]
	public string AssemblyName { get; set; } = string.Empty;

	[XmlAttribute("PublicKeyToken")]
	public string PublicKeyToken { get; set; } = string.Empty;

	[XmlAttribute("AssemblyVersion")]
	public string AssemblyVersion { get; set; } = string.Empty;

	[XmlAttribute("FileVersion")]
	public string FileVersion { get; set; } = string.Empty;

	internal static FrameworkFile Deserialize(XmlNode node)
	{
		var result = new FrameworkFile();
		if (node.Attributes is not null)
		{
			for (int i = 0; i < node.Attributes.Count; i++)
			{
				var attribute = node.Attributes[i];
				switch (attribute.Name)
				{
					case "Type":
						result.Type = attribute.Value ?? string.Empty;
						break;
					case "Path":
						result.Path = Path.Parse(attribute.Value) ?? new Path();
						break;
					case "Language":
						result.Language = attribute.Value ?? string.Empty;
						break;
					case "AssemblyName":
						result.AssemblyName = attribute.Value ?? string.Empty;
						break;
					case "PublicKeyToken":
						result.PublicKeyToken = attribute.Value ?? string.Empty;
						break;
					case "AssemblyVersion":
						result.AssemblyVersion = attribute.Value ?? string.Empty;
						break;
					case "FileVersion":
						result.FileVersion = attribute.Value ?? string.Empty;
						break;
					default:
						break;
				}
			}
		}

		return result;
	}
}