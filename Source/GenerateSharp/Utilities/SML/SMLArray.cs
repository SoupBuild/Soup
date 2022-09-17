// <copyright file="SMLArray.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public class SMLArray : IEquatable<SMLArray>
	{
		private List<SMLValue> _value;

		public SMLArray()
		{
			_value = new List<SMLValue>();
		}

		public SMLArray(List<SMLValue> value)

		{
			_value = value;
		}

		public SMLValue this[int key]
		{
			get
			{
				return _value[key];
			}
		}

		public int GetSize()
		{
			return _value.Count;
		}

		public List<SMLValue> GetValue()
		{
			return _value;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLArray);

		public bool Equals(SMLArray? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return Enumerable.SequenceEqual(this._value, rhs._value);
		}

		public override int GetHashCode() => (_value).GetHashCode();

		public static bool operator ==(SMLArray? lhs, SMLArray? rhs)
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

		public static bool operator !=(SMLArray? lhs, SMLArray? rhs) => !(lhs == rhs);
	}
}
