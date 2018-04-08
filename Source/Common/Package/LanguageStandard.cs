// <copyright company="Soup" file="LanguageStandard.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Runtime.Serialization;

namespace Soup
{
	public enum LanguageStandard
	{
		/// <summary>
		/// Standard not set
		/// </summary>
		[EnumMember(Value = "")]
		Default,

		/// <summary>
		/// C++ 11 Standard
		/// </summary>
		[EnumMember(Value = "11")]
		CPP11,

		/// <summary>
		/// C++ 14 Standard
		/// </summary>
		[EnumMember(Value = "14")]
		CPP14,

		/// <summary>
		/// C++ 17 Standard
		/// </summary>
		[EnumMember(Value = "17")]
		CPP17
	}
}
