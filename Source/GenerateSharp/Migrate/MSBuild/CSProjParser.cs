// <copyright file="CSProjParser.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.ObjectModel;
using System.Xml;
using Path = Opal.Path;

namespace Soup.Build.Migrate;

public class CSProjParser
{
	public string Sdk { get; private set; } = string.Empty;
	public string OutputType { get; private set; } = "Library";
	public string TargetFramework { get; private set; } = string.Empty;
	public bool PublishSingleFile { get; private set; }
	public bool SelfContained { get; private set; }
	public bool PublishReadyToRun { get; private set; }
	public bool PublishTrimmed { get; private set; }
	public string Platforms { get; private set; } = string.Empty;
	public Collection<ProjectReferenceItem> ProjectReferenceItems { get; } = [];
	public Collection<PackageReferenceItem> PackageReferenceItems { get; } = [];
	public Collection<CompileItem> CompileItems { get; } = [];
	public string NoWarn { get; private set; } = string.Empty;

	public CSProjParser()
	{
	}

	public void Deserialize(XmlReader reader)
	{
		var document = new XmlDocument();
		document.Load(reader);

		DeserializeNode(document, HandleDocumentNode, null);
	}

	private bool HandleDocumentNode(XmlNode node)
	{
		switch (node.Name)
		{
			case "Project":
				DeserializeNode(node, HandleProjectNode, HandleProjectAttribute);
				return true;
			default:
				return false;
		}
	}

	private bool HandleProjectAttribute(XmlAttribute attribute)
	{
		switch (attribute.Name)
		{
			case "Sdk":
				this.Sdk = attribute.Value;
				return true;
			default:
				return false;
		}
	}

	private bool HandleProjectNode(XmlNode node)
	{
		switch (node.Name)
		{
			case "PropertyGroup":
				DeserializeNode(node, HandlePropertyGroupNode, null);
				return true;
			case "ItemGroup":
				DeserializeNode(node, HandleItemGroupNode, null);
				return true;
			default:
				return false;
		}
	}

	private bool HandlePropertyGroupNode(XmlNode node)
	{
		switch (node.Name)
		{
			case "OutputType":
				this.OutputType = node.InnerText;
				return true;
			case "TargetFramework":
				this.TargetFramework = node.InnerText;
				return true;
			case "PublishSingleFile":
				this.PublishSingleFile = bool.Parse(node.InnerText);
				return true;
			case "SelfContained":
				this.SelfContained = bool.Parse(node.InnerText);
				return true;
			case "PublishReadyToRun":
				this.PublishReadyToRun = bool.Parse(node.InnerText);
				return true;
			case "PublishTrimmed":
				this.PublishTrimmed = bool.Parse(node.InnerText);
				return true;
			case "Platforms":
				this.Platforms = node.InnerText;
				return true;
			case "NoWarn":
				this.NoWarn = node.InnerText;
				return true;
			default:
				return false;
		}
	}

	private bool HandleItemGroupNode(XmlNode node)
	{
		switch (node.Name)
		{
			case "ProjectReference":
				var projectReference = new ProjectReferenceItem();
				DeserializeNode(node, null, (node) => HandleProjectReferenceAttribute(node, projectReference));
				this.ProjectReferenceItems.Add(projectReference);
				return true;
			case "PackageReference":
				var packageReference = new PackageReferenceItem();
				DeserializeNode(node, null, (node) => HandlePackageReferenceAttribute(node, packageReference));
				this.PackageReferenceItems.Add(packageReference);
				return true;
			case "Compile":
				var compile = new CompileItem();
				DeserializeNode(node, null, (node) => HandleCompileAttribute(node, compile));
				this.CompileItems.Add(compile);
				return true;
			default:
				return false;
		}
	}

	private static bool HandleProjectReferenceAttribute(XmlAttribute attribute, ProjectReferenceItem projectReference)
	{
		switch (attribute.Name)
		{
			case "Include":
				projectReference.Include = Path.Parse(attribute.Value);
				return true;
			default:
				return false;
		}
	}

	private static bool HandlePackageReferenceAttribute(XmlAttribute attribute, PackageReferenceItem packageReference)
	{
		switch (attribute.Name)
		{
			case "Include":
				packageReference.Include = attribute.Value;
				return true;
			case "Version":
				packageReference.Version = attribute.Value;
				return true;
			default:
				return false;
		}
	}

	private static bool HandleCompileAttribute(XmlAttribute attribute, CompileItem compile)
	{
		switch (attribute.Name)
		{
			case "Remove":
				compile.Remove = attribute.Value;
				return true;
			default:
				return false;
		}
	}

	private static void DeserializeNode(
		XmlNode node,
		Func<XmlNode, bool>? childHandler,
		Func<XmlAttribute, bool>? attributeHandler)
	{
		if (node.Attributes is not null)
		{
			for (int i = 0; i < node.Attributes.Count; i++)
			{
				var attribute = node.Attributes[i] ?? throw new InvalidOperationException("Null attribute");
				if (attributeHandler is null || !attributeHandler(attribute))
				{
					throw new InvalidOperationException($"Unknown attribute {attribute.Name} in {node.Name}");
				}
			}
		}

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