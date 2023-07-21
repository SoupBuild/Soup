// <copyright file="NuspecPackage.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;

namespace Swhere.Core.Nuget
{
	[XmlRoot("package")]
	public class NuspecPackage
	{
		[XmlElement("metadata")]
		public NuspecMetadata Metadata { get; set; } = new NuspecMetadata();

		public static NuspecPackage Deserialize(XmlReader reader)
		{
			var document = new XmlDocument();
			document.Load(reader);

			//Display the contents of the child nodes.
			NuspecPackage? result = null;
			if (document.HasChildNodes)
			{
				for (int i = 0; i < document.ChildNodes.Count; i++)
				{
					var child = document.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
					switch (child.Name)
					{
						case "package":
							result = DeserializePackage(child);
							break;
					}
				}
			}

			return result ?? throw new InvalidOperationException("Package is required");
		}

		private static NuspecPackage DeserializePackage(XmlNode node)
		{
			NuspecMetadata? metadata = null;
			if (node.HasChildNodes)
			{
				for (int i = 0; i < node.ChildNodes.Count; i++)
				{
					var child = node.ChildNodes[i] ?? throw new InvalidOperationException("Null child");
					switch (child.Name)
					{
						case "metadata":
							metadata = NuspecMetadata.Deserialize(child);
							break;
					}
				}
			}

			return new NuspecPackage()
			{
				Metadata = metadata ?? throw new InvalidOperationException("Metadata element is required"),
			};
		}
	}
}
