// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
	[Serializable]
	public class ProjectReference : Item
	{
		public ProjectReference() :
			this(null)
		{
		}

		public ProjectReference(string include)
		{
			Include = include;
		}

		[XmlAttribute("Include")]
		public string Include { get; set; }
	}
}
