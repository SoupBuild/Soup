// <copyright file="LanguageStandard.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System.Runtime.Serialization;

    /// <summary>
    /// The enumeration of language standards
    /// </summary>
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
