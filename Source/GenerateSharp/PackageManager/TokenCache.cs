// <copyright file="TokenCache.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.PackageManager
{
    using System.Diagnostics.CodeAnalysis;
    using System.IO;
    using System.Reflection;
    using System.Security.Cryptography;
    using Microsoft.Identity.Client;

    [SuppressMessage("Interoperability", "CA1416:Validate platform compatibility", Justification = "Will investigate ASP.Net Data Protection when supporting more platforms.")]
    internal static class TokenCache
    {
        /// <summary>
        /// Path to the token cache
        /// </summary>
        public static readonly string CacheFilePath = Assembly.GetExecutingAssembly().Location + ".msalcache.bin";

        private static readonly object FileLock = new object();

        public static void BeforeAccessNotification(TokenCacheNotificationArgs args)
        {
            lock (FileLock)
            {
                args.TokenCache.DeserializeMsalV3(
                    File.Exists(CacheFilePath)
                    ? ProtectedData.Unprotect(File.ReadAllBytes(CacheFilePath), null, DataProtectionScope.CurrentUser)
                    : null);
            }
        }

        public static void AfterAccessNotification(TokenCacheNotificationArgs args)
        {
            // if the access operation resulted in a cache update
            if (args.HasStateChanged)
            {
                lock (FileLock)
                {
                    // reflect changes in the persistent store
                    File.WriteAllBytes(
                        CacheFilePath,
                        ProtectedData.Protect(args.TokenCache.SerializeMsalV3(), null, DataProtectionScope.CurrentUser));
                }
            }
        }

        public static void Bind(ITokenCache tokenCache)
        {
            tokenCache.SetBeforeAccess(BeforeAccessNotification);
            tokenCache.SetAfterAccess(AfterAccessNotification);
        }
    }
}
