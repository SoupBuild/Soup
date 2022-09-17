// <copyright file="SMLDocument.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The SML Document
	/// </summary>
	public class SMLDocument
	{
		private SMLTable _root;

		public SMLDocument(SMLTable root)
		{
			_root = root;
		}

		public SMLTable GetRoot()
		{
			return _root;
		}
	}
}
