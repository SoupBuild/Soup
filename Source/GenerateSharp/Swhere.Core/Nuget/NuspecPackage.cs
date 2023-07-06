// <copyright file="NuspecPackage.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Xml.Serialization;

namespace Swhere.Core.Nuget
{
	[XmlRoot("package")]
	public class NuspecPackage
	{
		[XmlElement("metadata")]
		public NuspecMetadata Metadata { get; set; } = new NuspecMetadata();
	}
}
