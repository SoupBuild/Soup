// <copyright company="Soup" file="ProjectConfiguration.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System;
	using System.Xml.Serialization;

	/// <summary>
	/// Project Configuration Element
	/// </summary>
	[Serializable]
	public class ProjectConfiguration : Item
	{
		public ProjectConfiguration() :
			this(string.Empty, string.Empty)
		{
		}

		public ProjectConfiguration(string configuration, string platform)
		{
			Configuration = configuration;
			Platform = platform;
		}

		[XmlAttribute("Include")]
		public string Include
		{
			get { return $"{Configuration}|{Platform}"; }
			set { }
		}

		[XmlElement("Configuration")]
		public string Configuration { get; set; }

		[XmlElement("Platform")]
		public string Platform { get; set; }
	}
}
