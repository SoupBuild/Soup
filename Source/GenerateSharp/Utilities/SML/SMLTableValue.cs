// <copyright file="SMLTableValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities;

public class SMLTableValue : IEquatable<SMLTableValue>
{
	public SMLToken Key { get; set; }
	public string KeyContent { get; set; }
	public SMLToken Colon { get; set; }
	public SMLValue Value { get; set; }
	public List<SMLToken> Delimiter { get; set; }

	public SMLTableValue(
		SMLToken key,
		string keyContent,
		SMLToken colon,
		SMLValue value)
	{
		Key = key;
		KeyContent = keyContent;
		Colon = colon;
		Value = value;
		Delimiter = new List<SMLToken>();
	}

	public SMLTableValue(
		SMLToken key,
		string keyContent,
		SMLToken colon,
		SMLValue value,
		List<SMLToken> delimiter)
	{
		Key = key;
		KeyContent = keyContent;
		Colon = colon;
		Value = value;
		Delimiter = delimiter;
	}

	public override bool Equals(object? obj) => this.Equals(obj as SMLTableValue);

	public bool Equals(SMLTableValue? rhs)
	{
		if (rhs is null)
			return false;

		// Optimization for a common success case.
		if (object.ReferenceEquals(this, rhs))
			return true;

		// Return true if the fields match.
		return this.KeyContent == rhs.KeyContent &&
			this.Value == rhs.Value;
	}

	public override int GetHashCode() => (Value).GetHashCode();

	public static bool operator ==(SMLTableValue? lhs, SMLTableValue? rhs)
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

	public static bool operator !=(SMLTableValue? lhs, SMLTableValue? rhs) => !(lhs == rhs);
}