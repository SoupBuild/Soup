// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.IO;

namespace Soup
{
    internal static class BuildRequiredChecker
    {
        public static bool IsSourceFileOutdated(string rootPath, BuildState buildState, string targetFile, string sourceFile, IList<string> dependencies)
        {
            var knownFiles = buildState.GetKnownFileLookup();
            var sourceFiles = new List<string>();
            sourceFiles.Add(sourceFile);

            // Copy the dependencies and add all transient dependencies to the collection
            var dependencyClosure = new List<string>();
            dependencyClosure.AddRange(dependencies);
            dependencyClosure.Add(sourceFile);

            if (TryBuildIncludeSet(sourceFiles, knownFiles, dependencyClosure))
            {
                return IsOutdated(rootPath, targetFile, dependencyClosure);
            }
            else
            {
                Log.Verbose($"Unable to resolve include set from previous build.");
                return true;
            }
        }

        public static bool IsOutdated(string rootPath, string targetFile, IList<string> dependencies)
        {
            // Verify the output file exists
            var relativeOutputFile = Path.Combine(rootPath, targetFile);
            if (!File.Exists(relativeOutputFile))
            {
                Log.Verbose($"Output target does not exist: {targetFile}");
                return true;
            }

            var outputFileLastWriteTime = File.GetLastWriteTime(relativeOutputFile);
            foreach (var dependency in dependencies)
            {
                var relativeDependency = Path.Combine(rootPath, dependency);
                if (!File.Exists(relativeDependency))
                {
                    Log.Verbose($"Dependency file missing, rebuild required.");
                    Log.Verbose($"[{dependency}] -> [{targetFile}].");
                    return true;
                }

                var dependencyLastWriteTime = File.GetLastWriteTime(relativeDependency);
                if (dependencyLastWriteTime > outputFileLastWriteTime)
                {
                    Log.Verbose($"Dependency altered after target, rebuild required.");
                    Log.Verbose($"[{dependency}] -> [{targetFile}].");
                    return true;
                }
            }

            return false;
        }

        private static bool TryBuildIncludeSet(IList<string> sourceFiles, IDictionary<string, FileInfo> knownFiles, List<string> set)
        {
            foreach (var file in sourceFiles)
            {
                if (knownFiles.TryGetValue(file, out var info))
                {
                    // Add all of the new files and recursively check their includes
                    List<string> newIncludes = new List<string>();
                    foreach (var include in info.Includes)
                    {
                        if (set.Contains(include))
                        {
                            set.Add(include);
                            newIncludes.Add(include);
                        }
                    }

                    // Build up the child includes
                    if (!TryBuildIncludeSet(newIncludes, knownFiles, set))
                    {
                        return false;
                    }
                }
                else
                {
                    Log.Verbose($"Missing file info: {file}");
                    return false;
                }
            }

            return true;
        }
    }
}
