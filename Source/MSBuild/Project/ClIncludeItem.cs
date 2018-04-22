// <copyright company="Soup" file="ClIncludeItem.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System.Xml;
	using System.Xml.Serialization;

	/// <summary>
	/// Cl Include Item element
	/// </summary>
	public class ClIncludeItem : Item
	{
		public ClIncludeItem() :
			this(null)
		{
		}

		public ClIncludeItem(string include)
		{
			Include = include;
		}

		[XmlAttribute("Include")]
		public string Include { get; set; }
	}
}
