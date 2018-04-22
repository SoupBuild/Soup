// <copyright company="Soup" file="Property.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	/// <summary>
	/// Property Element
	/// </summary>
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
