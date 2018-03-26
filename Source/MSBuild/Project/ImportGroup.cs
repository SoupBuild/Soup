// <copyright company="Soup" file="ImportGroup.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.VisualStudioBuild
{
	using System;
	using System.Collections.Generic;
	using System.Xml.Serialization;

	/// <summary>
	/// Import group element
	/// </summary>
	[Serializable]
	public class ImportGroup : ProjectElement
	{
		public ImportGroup()
		{
		}

		[XmlAttribute("Label")]
		public string Label { get; set; }

		[XmlElement("Import")]
		public List<Import> Imports { get; set; }
	}
}
