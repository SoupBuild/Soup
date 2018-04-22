// <copyright company="Soup" file="Import.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System;
	using System.Xml.Serialization;

	/// <summary>
	/// Import element
	/// </summary>
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
