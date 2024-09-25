// <copyright file="ValueType.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Diagnostics.CodeAnalysis;

namespace Soup.Build.Utilities;

[SuppressMessage("Naming", "CA1720:Identifier contains type name", Justification = "Is type")]
[SuppressMessage("Design", "CA1008:Enums should have zero value", Justification = "NA")]
public enum ValueType
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
	/// Version
	/// </summary>
	Version = 7,

	/// <summary>
	/// PackageReference
	/// </summary>
	PackageReference = 8,

	/// <summary>
	/// LanguageReference
	/// </summary>
	LanguageReference = 9,
}