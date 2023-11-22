// <copyright file="ValueType.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Diagnostics.CodeAnalysis;

namespace Soup.Build.Utilities;

[SuppressMessage("Naming", "CA1720:Identifier contains type name", Justification = "Is type")]
public enum ValueType : uint
{
	/// <summary>
	/// Table
	/// </summary>
	Table = 1,

	/// <summary>
	/// List
	/// </summary>
	List = 2,

	/// <summary>
	/// String
	/// </summary>
	String = 3,

	/// <summary>
	/// Integer
	/// </summary>
	Integer = 4,

	/// <summary>
	/// Float
	/// </summary>
	Float = 5,

	/// <summary>
	/// Boolean
	/// </summary>
	Boolean = 6,

	/// <summary>
	/// Empty
	/// </summary>
	Empty = 7,
}