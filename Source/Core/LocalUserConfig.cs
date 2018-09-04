// <copyright file="LocalUserConfig.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System;
    using System.IO;
    using System.Runtime.InteropServices;

    /// <summary>
    /// The local user configuration
    /// </summary>
    public class LocalUserConfig
    {
        /// <summary>
        /// Gets the package store
        /// </summary>
        public string PackageStore
        {
            get
            {
                if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
                {
                    return @"C:\SoupKitchen";
                }
                else
                {
                    var homePath = Environment.GetEnvironmentVariable("HOME");
                    return Path.Combine(homePath, ".soup");
                }
            }
        }
    }
}
