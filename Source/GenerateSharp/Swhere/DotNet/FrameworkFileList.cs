// <copyright file="FrameworkFileList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;

namespace Soup.Build.Discover;

[XmlRoot("FileList")]
public class FrameworkFileList
{
	[XmlElement("File")]
	public IList<FrameworkFile> Files { get; init; } = [];

	public static FrameworkFileList Deserialize(XmlReader reader)
	{
		var document = new XmlDocument();
		document.Load(reader);

		// Display the contents of the child nodes.
		FrameworkFileList? result = null;
		if (document.HasChildNodes)
		{
			for (int i = 0; i < document.ChildNodes.Count; i++)
			{
				var child = document.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
				switch (child.Name)
				{
					case "FileList":
						result = DeserializeFileList(child);
						break;
					default:
						break;
				}
			}
		}

		return result ?? throw new InvalidOperationException("Package is required");
	}

	private static FrameworkFileList DeserializeFileList(XmlNode node)
	{
		var result = new FrameworkFileList();
		if (node.HasChildNodes)
		{
			for (int i = 0; i < node.ChildNodes.Count; i++)
			{
				var child = node.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
				switch (child.Name)
				{
					case "File":
						var dependency = FrameworkFile.Deserialize(child);
						result.Files.Add(dependency);
						break;
					default:
						break;
				}
			}
		}

		return result;
	}
}