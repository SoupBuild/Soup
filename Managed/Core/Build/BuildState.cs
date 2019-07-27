// <copyright file="BuildState.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Newtonsoft.Json;
using Soup.Compiler;
using System.Collections.Generic;
using System.Linq;

namespace Soup
{
    [JsonObject]
    internal class FileInfo
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="FileInfo"/> class.
        /// </summary>
        [JsonConstructor]
        public FileInfo(string file)
        {
            File = file;
            Includes = new List<string>();
        }

        [JsonProperty("file")]
        public string File { get; set; }

        [JsonProperty("includes")]
        public IList<string> Includes;
    }

    [JsonObject]
    internal class BuildState
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="BuildState"/> class.
        /// </summary>
        [JsonConstructor]
        public BuildState()
        {
            KnownFiles = new List<FileInfo>();
        }

        [JsonProperty("knownFiles")]
        public IList<FileInfo> KnownFiles;

        /// <summary>
        /// Helper to load a lookup dictionary for all known files
        /// </summary>
        public IDictionary<string, FileInfo> GetKnownFileLookup()
        {
            var lookup = new Dictionary<string, FileInfo>();
            foreach (var file in KnownFiles)
            {
                var insertResult = lookup.TryAdd(file.File, file);
                if (!insertResult)
                    Log.Warning("Found second copy of file...");
            }

            return lookup;
        }

        /// <summary>
        /// Add the file include tree to the build state
        /// </summary>
        public void UpdateIncludeTree(IList<HeaderInclude> includeTree)
        {
            var knownFiles = new Dictionary<string, FileInfo>();

            // Add all of the new files in the tree recursively
            AddIncludeTree(includeTree, knownFiles);

            // Add in any missing files from the previous known files list
            // Note: this is done after parsing the new tree to allow updates to include lists
            foreach (var file in KnownFiles)
            {
                bool ignore = knownFiles.TryAdd(file.File, file);
            }

            // Replace the current set of know files
            KnownFiles = knownFiles.Values.ToList();
        }

        /// <summary>
        /// Recursive helper method to all add values down the tree
        /// </summary>
        private void AddIncludeTree(IList<HeaderInclude> includeTree, IDictionary<string, FileInfo> knownFiles)
        {
            foreach (var file in includeTree)
            {
                // Add the current file in not known
                if (!knownFiles.ContainsKey(file.Filename))
                {
                    var fileInfo = new FileInfo(file.Filename)
                    {
                        Includes = file.Includes.Select(include => include.Filename).ToList(),
                    };

                    knownFiles.Add(file.Filename, fileInfo);

                    // Check if we should recursively add includes
                    if (file.Includes.Count > 0)
                    {
                        AddIncludeTree(file.Includes, knownFiles);
                    }
                }
            }
        }
    }
}
