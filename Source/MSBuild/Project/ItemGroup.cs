// <copyright company="Soup" file="ItemGroup.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System;
	using System.Collections.Generic;
	using System.Xml;
	using System.Xml.Serialization;

	/// <summary>
	/// Item Group Element
	/// </summary>
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
