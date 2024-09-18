// <copyright file="SMLArrayValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities;

public class SMLArrayValue : IEquatable<SMLArrayValue>
{
	public SMLValue Value { get; set; }
	public IList<SMLToken> Delimiter { get; init; }

	public SMLArrayValue(SMLValue value)
	{
		Value = value;
		Delimiter = [];
	}

	public SMLArrayValue(
		SMLValue value,
		IList<SMLToken> delimiter)
	{
		Value = value;
		Delimiter = delimiter;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLArrayValue);
	}

	public bool Equals(SMLArrayValue? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		return Value == other.Value;
	}

	public override int GetHashCode()
	{
		return Value.GetHashCode();
	}

	public static bool operator ==(SMLArrayValue? lhs, SMLArrayValue? rhs)
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

	public static bool operator !=(SMLArrayValue? lhs, SMLArrayValue? rhs)
	{
		return !(lhs == rhs);
	}
}