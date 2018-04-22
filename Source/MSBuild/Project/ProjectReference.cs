// <copyright company="Soup" file="ProjectReference.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System;
	using System.Xml.Serialization;

	/// <summary>
	/// Project Reference Object
	/// </summary>
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
