// <copyright company="Soup" file="ItemDefinitionGroup.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System;
	using System.Xml.Serialization;

	/// <summary>
	/// Item Definition Group Element
	/// </summary>
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
