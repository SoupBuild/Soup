# Simple Build Extension
This is a console application that has a custom build extension that alters the build state. The custom build Tasks will run before and after the core Build Task and will simply print a nice hello message.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/CSharp/SimpleBuildExtension)

## Extension/Recipe.toml
The Recipe file that defines the build extension dynamic library "Samples.SimpleBuildExtension.Extension" that will register new build tasks.
```
Name = "Samples.SimpleBuildExtension.Extension"
Language = "C#"
Version = "1.0.0"
Source = [
    "CustomBuildTask.cs"
]

[Dependencies]
Runtime = [
    { Reference = "Soup.Build@0.1.3", ExcludeRuntime = true },
    { Reference = "Soup.Build.Extensions@0.1.8" },
    { Reference = "Opal@1.0.1" },
]
```

## Extension/CustomBuildTask.cs
A C# file defining a custom build Task that will run before the build definition and sets a custom preprocessor definition to show how a user can alter the build state through an extension.
```
// <copyright file="CustomBuildTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace SimpleBuildExtension.Extension
{
    using System.Collections.Generic;
    using Soup.Build;

    public class CustomBuildTask : IBuildTask
    {
        private IBuildState buildState;
        private IValueFactory factory;

        /// <summary>
        /// Get the run before list
        /// </summary>
        public static IReadOnlyList<string> RunBeforeList => new List<string>()
        {
            "BuildTask",
        };

        /// <summary>
        /// Get the run after list
        /// </summary>
        public static IReadOnlyList<string> RunAfterList => new List<string>()
        {
        };

        public CustomBuildTask(IBuildState buildState, IValueFactory factory)
        {
            this.buildState = buildState;
            this.factory = factory;
        }

        public void Execute()
        {
            this.buildState.LogTrace(TraceLevel.HighPriority, "Running Before Build!");

            // Get the build table
            var buildTable = this.buildState.ActiveState["Build"].AsTable();

            // As a sample of what a build extension can do we set a new
            // preprocessor definition to influence the build
            var preprocessorDefinitions = new List<string>()
            {
                "SPECIAL_BUILD",
            };
            buildTable.EnsureValueList(this.factory, "PreprocessorDefinitions").Append(this.factory, preprocessorDefinitions);
        }
    }
}
```

## Executable/Recipe.toml
The Recipe file that defines the executable "SimpleBuildExtension.Executable". The one interesting part is the relative path reference to the custom build extension through "Build" Dependencies.
```
Name = "Samples.SimpleBuildExtension.Executable"
Language = "C#"
Type = "Executable"
Version = "1.0.1"
Source = [
    "Program.cs"
]

[Dependencies]
Build = [
    "../Extension/"
]
```

## Executable/Program.cs
A simple C# Program method that prints our "Hello World, Soup Style!" only if the build extension was able to set the custom preprocessor definition "SPECIAL_BUILD".
```
// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace SimpleBuildExtension.Executable
{
    using System;

    public class Program
    {
        public static void Main(string[] args)
        {
            #if SPECIAL_BUILD
                Console.WriteLine("Hello World, Soup Style!");
            #else
                Console.WriteLine("Hello World...");
            #endif
        }
    }
}


```

## .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```
