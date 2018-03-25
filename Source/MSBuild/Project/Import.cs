// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
	[Serializable]
	public class Import : ProjectElement
	{
		public Import() :
			this(null)
		{
		}

		public Import(string project)
		{
			Project = project;
		}

		[XmlAttribute("Project")]
		public string Project { get; set; }

		[XmlAttribute("Condition")]
		public string Condition { get; set; }

		[XmlAttribute("Label")]
		public string Label { get; set; }
	}
}
