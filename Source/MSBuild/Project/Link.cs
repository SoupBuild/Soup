// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
	[Serializable]
	public class Link
	{
		public Link()
		{
		}

		[XmlElement]
		public string SubSystem { get; set; }

		[XmlElement]
		public bool? GenerateDebugInformation { get; set; }
		public bool GenerateDebugInformationSpecified => GenerateDebugInformation.HasValue;

		[XmlElement]
		public bool? EnableCOMDATFolding { get; set; }
		public bool EnableCOMDATFoldingSpecified => EnableCOMDATFolding.HasValue;

		[XmlElement]
		public bool? OptimizeReferences { get; set; }
		public bool OptimizeReferencesSpecified => OptimizeReferences.HasValue;
	}
}
