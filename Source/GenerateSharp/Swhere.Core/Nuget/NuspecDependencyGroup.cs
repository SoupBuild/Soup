// <copyright file="NuspecDependencyGroup.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Xml.Serialization;

namespace Swhere.Core.Nuget
{
	public class NuspecDependencyGroup : NuspecDependencyBase
	{
		[XmlAttribute("targetFramework")]
		public string TargetFramework { get; set; } = string.Empty;

		[XmlElement("dependency")]
		public List<NuspecDependency> Dependencies { get; set; } = new List<NuspecDependency>();
	}
}
