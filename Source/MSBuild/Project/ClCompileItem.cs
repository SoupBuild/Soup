// <copyright company="Soup" file="ClCompileItem.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System.Xml;
	using System.Xml.Serialization;

	/// <summary>
	/// Cl Compile item element
	/// </summary>
	public class ClCompileItem : Item
	{
		public ClCompileItem() :
			this(null)
		{
		}

		public ClCompileItem(string include)
		{
			Include = include;
		}

		[XmlAttribute("Include")]
		public string Include { get; set; }
	}
}
