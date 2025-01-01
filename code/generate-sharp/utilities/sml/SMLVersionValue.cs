// <copyright file="SMLStringValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;

namespace Soup.Build.Utilities;

public class SMLVersionValue : IEquatable<SMLVersionValue>
{
	public SemanticVersion Value { get; set; }

	public SMLToken Content { get; set; }

	public SMLVersionValue(
		SemanticVersion value)
	{
		this.Value = value;
		this.Content = new SMLToken($"{value}");
	}

	public SMLVersionValue(
		SemanticVersion value,
		SMLToken content)
	{
		this.Value = value;
		this.Content = content;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLVersionValue);
	}

	public bool Equals(SMLVersionValue? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		return this.Value == other.Value;
	}

	public override int GetHashCode()
	{
		return this.Value.GetHashCode();
	}

	public static bool operator ==(SMLVersionValue? lhs, SMLVersionValue? rhs)
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

	public static bool operator !=(SMLVersionValue? lhs, SMLVersionValue? rhs)
	{
		return !(lhs == rhs);
	}
}