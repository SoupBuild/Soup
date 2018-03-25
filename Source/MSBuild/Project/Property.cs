// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.VisualStudioBuild
{
	public struct Property
	{
		public Property(string name, string value)
		{
			Name = name;
			Value = value;
		}

		public string Name { get; set; }
		public string Value { get; set; }
	}
}
