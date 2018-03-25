// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
	[Serializable]
	public class ItemDefinitionGroup : ProjectElement
	{
		public ItemDefinitionGroup()
		{
		}

		[XmlAttribute("Condition")]
		public string Condition { get; set; }

		[XmlElement("ClCompile")]
		public ClCompile ClCompile { get; set; }

		[XmlElement("Link")]
		public Link Link { get; set; }
	}
}
