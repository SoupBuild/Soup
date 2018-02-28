// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
	[Serializable]
	public class ItemGroup : ProjectElement
	{
		public ItemGroup()
		{
		}

		[XmlAttribute("Label")]
		public string Label { get; set; }

		[XmlElement("ClCompile", typeof(ClCompileItem))]
		[XmlElement("ClInclude", typeof(ClIncludeItem))]
		[XmlElement("ProjectConfiguration", typeof(ProjectConfiguration))]
		[XmlElement("ProjectReference", typeof(ProjectReference))]
		public List<Item> Items { get; set; }
	}
}
