// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
	/// <summary>
	/// Visual Compiler Project
	/// </summary>
	[XmlRoot("Project", Namespace = "http://schemas.microsoft.com/developer/msbuild/2003")]
	public class Project
	{
		public Project()
		{
			Namespaces = new XmlSerializerNamespaces(
				   new XmlQualifiedName[]
				   {
					new XmlQualifiedName(string.Empty, "http://schemas.microsoft.com/developer/msbuild/2003")
				   });
		}

		[XmlAttribute("DefaultTargets")]
		public string DefaultTargets { get; set; }
		[XmlAttribute("ToolsVersion")]
		public string ToolsVersion { get; set; }

		[XmlNamespaceDeclarations]
		public XmlSerializerNamespaces Namespaces { get; set; }

		[XmlElement("Import", typeof(Import))]
		[XmlElement("ImportGroup", typeof(ImportGroup))]
		[XmlElement("ItemDefinitionGroup", typeof(ItemDefinitionGroup))]
		[XmlElement("ItemGroup", typeof(ItemGroup))]
		[XmlElement("PropertyGroup", typeof(PropertyGroup))]
		public List<ProjectElement> Elements { get; set; }
	}
}
