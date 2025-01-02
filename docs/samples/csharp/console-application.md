# C# Console Application
This is a console application with the minimal amount of code to get up and running in C#.

[Source](https://github.com/soup-build/soup/tree/main/samples/csharp/console-application)

## Recipe.sml
The Recipe file that defines the static library "Samples.CSharp.ConsoleApplication".
```sml
Name: 'Samples.CSharp.ConsoleApplication'
Language: (C#@0)
Type: 'Executable'
Version: 1.0.0
Source: [
  'Program.cs'
]
```

## PackageLock.sml
The package lock that was generated to capture the unique build dependencies required to build this project.
```sml
Version: 5
Closures: {
  Root: {
    'C#': {
      'Samples.CSharp.ConsoleApplication': { Version: '../ConsoleApplication', Build: 'Build0', Tool: 'Tool0' }
    }
  }
  Build0: {
    Wren: {
      'Soup|CSharp': { Version: 0.15.2 }
    }
  }
  Tool0: {
    'C++': {
      'mwasplund|copy': { Version: 1.1.0 }
      'mwasplund|mkdir': { Version: 1.1.0 }
    }
  }
}
```

## Program.cs
A C# Program file that implements the main method.
```C#
using System;

namespace Samples.CSharp.ConsoleApplication
{
  public class Program
  {
    public static void Main(string[] args)
    {
      Console.WriteLine("Hello World, Soup Style!");
    }
  }
}
```

## .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```
