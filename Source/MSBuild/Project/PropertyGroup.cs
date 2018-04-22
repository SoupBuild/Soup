// <copyright company="Soup" file="PropertyGroup.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System;
	using System.Collections.Generic;
	using System.Xml;
	using System.Xml.Schema;
	using System.Xml.Serialization;

	/// <summary>
	/// Property Group Element
	/// </summary>
	public class PropertyGroup : ProjectElement, IXmlSerializable
	{
		public PropertyGroup()
		{
		}

		[XmlAttribute("Condition")]
		public string Condition { get; set; }

		[XmlAttribute("Label")]
		public string Label { get; set; }

		public List<Property> Properties { get; set; }

		public XmlSchema GetSchema()
		{
			return null;
		}

		public void ReadXml(XmlReader reader)
		{
			throw new NotImplementedException();
		}

		public void WriteXml(XmlWriter writer)
		{
			if (Condition != null)
			{
				writer.WriteAttributeString("Condition", Condition);
			}

			if (Label != null)
			{
				writer.WriteAttributeString("Label", Label);
			}

			foreach (var property in Properties)
			{
				writer.WriteElementString(property.Name, property.Value);
			}
		}
	}
}
