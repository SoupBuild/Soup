// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
	public class AdditionalIncludeDirectories : ClCompileItem, IXmlSerializable
	{
		public AdditionalIncludeDirectories()
		{
		}

		public List<string> Includes { get; set; }

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
			bool indent = false;
			string contentIndentation = string.Empty;
			string indentation = string.Empty;
			var textWriter = writer as XmlTextWriter;
			if (textWriter != null)
			{
				indent = textWriter.Formatting == Formatting.Indented;
				if (indent)
				{
					indentation = new string(textWriter.IndentChar, 3 * textWriter.Indentation);
					contentIndentation = new string(textWriter.IndentChar, 4 * textWriter.Indentation);
				}
			}
			
			var content = new StringBuilder();
			foreach (var include in Includes)
			{
				if (indent)
				{
					content.Append("\n");
					content.Append(contentIndentation);
				}

				content.Append(include);
				content.Append(";");
			}

			if (indent)
			{
				content.Append("\n");
				content.Append(indentation);
			}

			writer.WriteValue(content.ToString());
		}
	}
}