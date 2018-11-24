// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.IO;

namespace Soup
{
    internal static class BuildRequiredChecker
    {
        public static bool IsOutdated(string rootPath, string outputFile, IList<string> dependencies)
        {
            // Verify the output file exists
            var relativeOutputFile = Path.Combine(rootPath, outputFile);
            if (!File.Exists(relativeOutputFile))
            {
                Log.Verbose($"Output target does not exist: {outputFile}");
                return true;
            }

            var outputFileLastWriteTime = File.GetLastWriteTime(relativeOutputFile);
            foreach (var dependency in dependencies)
            {
                var relativeDependency = Path.Combine(rootPath, dependency);
                var dependencyLastWriteTime = File.GetLastWriteTime(relativeDependency);
                if (dependencyLastWriteTime > outputFileLastWriteTime)
                {
                    Log.Verbose($"Dependency altered after target, rebuild required.");
                    Log.Verbose($"[{dependency}] -> [{outputFile}].");
                    return true;
                }
            }

            return false;
        }
    }
}
