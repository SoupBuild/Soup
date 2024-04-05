// <copyright file="CSProjParser.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Xml;

namespace Soup.Build.Migrate;

public class CSProjParser
{
	public string OutputType { get; private set; } = "Library";

	public CSProjParser()
	{
	}

	public void Deserialize(XmlReader reader)
	{
		var document = new XmlDocument();
		document.Load(reader);

		DeserializeNode(document, HandleDocumentNodes);
	}

	private bool HandleDocumentNodes(XmlNode node)
	{
		switch (node.Name)
		{
			case "Project":
				DeserializeNode(node, HandleProjectNodes);
				return true;
			default:
				return false;
		}
	}

	private bool HandleProjectNodes(XmlNode node)
	{
		switch (node.Name)
		{
			case "PropertyGroup":
				DeserializeNode(node, HandlePropertyGroupNodes);
				return true;
			case "ItemGroup":
				DeserializeNode(node, HandleItemGroupNodes);
				return true;
			default:
				return false;
		}
	}

	private bool HandlePropertyGroupNodes(XmlNode node)
	{
		switch (node.Name)
		{
			case "OutputType":
				this.OutputType = node.InnerText;
				return true;
			case "TargetFramework":
				return true;
			case "PublishSingleFile":
				return true;
			case "SelfContained":
				return true;
			case "PublishReadyToRun":
				return true;
			case "PublishTrimmed":
				return true;
			case "Platforms":
				return true;
			default:
				return false;
		}
	}

	private bool HandleItemGroupNodes(XmlNode node)
	{
		switch (node.Name)
		{
			default:
				return false;
		}
	}

	private static void DeserializeNode(XmlNode node, Func<XmlNode, bool>? childHandler)
	{
		if (node.HasChildNodes)
		{
			for (int i = 0; i < node.ChildNodes.Count; i++)
			{
				var child = node.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
				if (childHandler is null || !childHandler(child))
				{
					throw new InvalidOperationException($"Unknown child {child.Name} in {node.Name}");
				}
			}
		}
	}
}