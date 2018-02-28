// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Xml;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
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
