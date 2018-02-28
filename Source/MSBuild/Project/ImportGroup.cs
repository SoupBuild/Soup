// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
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
