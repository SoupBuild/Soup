# C Build Extension
This is a console application that has a custom build extension that alters the build state. The custom build Tasks will run before and after the core Build Task and will simply print a nice hello message.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/C/BuildExtension)

## Extension/Recipe.sml
The Recipe file that defines the build extension dynamic library "Samples.C.BuildExtension.Extension" that will register new build tasks.
```
Name: "Samples.C.BuildExtension.Extension"
Language: "Wren|0.1"
Version: "1.0.0"
Source: [
    "CustomBuildTask.wren"
]

Dependencies: {
    Runtime: [
        "Soup.Build.Utils@0.4.0"
    ]
}
```

## Extension/PackageLock.sml
The package lock that was generated to capture the unique dependencies required to build this project.
```
Version: 4
Version: 4
Closures: {
    Root: {
        Wren: [
            { Name: "Samples.C.BuildExtension.Extension", Version: "./", Build: "Build0", Tool: "Tool0" }
            { Name: "Soup.Build.Utils", Version: "0.5.0", Build: "Build0", Tool: "Tool0" }
        ]
    }
    Build0: {
        Wren: [
            { Name: "Soup.Wren", Version: "0.2.0" }
        ]
    }
    Tool0: {
        "C++": [
            { Name: "copy", Version: "1.0.0" }
            { Name: "mkdir", Version: "1.0.0" }
        ]
    }
}
```

## Extension/CustomBuildTask.wren
A Wren file defining a custom build Task that will run before the build definition and sets a custom preprocessor definition to show how a user can alter the build state through an extension.
```
// <copyright file="CustomBuildTask.wren" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

import "soup" for Soup, SoupTask
import "Soup.Build.Utils:./ListExtensions" for ListExtensions
import "Soup.Build.Utils:./MapExtensions" for MapExtensions

class CustomBuildTask is SoupTask {
    /// <summary>
    /// Get the run before list
    /// </summary>
    static runBefore { [
        "BuildTask"
    ] }

    /// <summary>
    /// Get the run after list
    /// </summary>
    static runAfter { [] }

    /// <summary>
    /// Core Evaluate
    /// </summary>
    static evaluate() {
        Soup.info("Running Before Build!")

        // Get the build table
        var buildTable = MapExtensions.EnsureTable(Soup.activeState, "Build")

        // As a sample of what a build extension can do we set a new
        // preprocessor definition to influence the build
        var preprocessorDefinitions = [
            "SPECIAL_BUILD",
        ]
        
        ListExtensions.Append(
            MapExtensions.EnsureList(buildTable, "PreprocessorDefinitions"),
            preprocessorDefinitions)
    }
}
```

## Executable/Recipe.sml
The Recipe file that defines the executable "Samples.C.BuildExtension.Executable". The one interesting part is the relative path reference to the custom build extension through "Build" Dependencies.
```
Name: "Samples.C.SimpleBuildExtension.Executable"
Language: "C|0.1"
Type: "Executable"
Version: "1.0.0"
Source: [
    "Main.c"
]

Dependencies: {
    Build: [
        "../Extension/"
    ]
}
```

## Executable/PackageLock.sml
The package lock that was generated to capture the unique build dependencies required to build this project.
```
Version: 4
Closures: {
    Root: {
        C: [
            { Name: "Samples.C.SimpleBuildExtension.Executable", Version: "./", Build: "Build0", Tool: "Tool0" }
        ]
    }
    Build0: {
        Wren: [
            { Name: "Samples.C.BuildExtension.Extension", Version: "../Extension/" }
            { Name: "Soup.C", Version: "0.1.0" }
        ]
    }
    Tool0: {
        "C++": [
            { Name: "copy", Version: "1.0.0" }
            { Name: "mkdir", Version: "1.0.0" }
        ]
    }
}
```

## Executable/Main.c
A simple main method that prints our "Hello World, Soup Style!" only if the build extension was able to set the custom preprocessor definition "SPECIAL_BUILD".
```
#include <stdio.h>

int main()
{
#ifdef SPECIAL_BUILD
    printf("Hello World, Soup Style!");
#else
    printf("Hello World...");
#endif
    return 0;
}
```

## .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```
