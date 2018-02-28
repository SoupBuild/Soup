// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Xml;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
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
