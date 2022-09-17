// <copyright file="SMLDocument.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public class SMLDocument : IEquatable<SMLDocument>
	{
		public Dictionary<string, SMLTableValue> Values { get; set; }

		public SMLDocument()
		{
			Values = new Dictionary<string, SMLTableValue>();
		}

		public SMLDocument(Dictionary<string, SMLTableValue> values)
		{
			Values = values;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLDocument);

		public bool Equals(SMLDocument? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return Enumerable.SequenceEqual(this.Values, rhs.Values);
		}

		public override int GetHashCode() => (Values).GetHashCode();

		public static bool operator ==(SMLDocument? lhs, SMLDocument? rhs)
		{
			if (lhs is null)
			{
				if (rhs is null)
					return true;
				else
					return false;
			}

			return lhs.Equals(rhs);
		}

		public static bool operator !=(SMLDocument? lhs, SMLDocument? rhs) => !(lhs == rhs);
	}
}
