# C# Console Application
This is a console application with the minimal amount of code to get up and running in C#.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/CSharp/ConsoleApplication)

## Recipe.toml
The Recipe file that defines the static library ""Samples.CSharp.ConsoleApplication".
```
Name = "Samples.CSharp.ConsoleApplication"
Language = "C#|0.1"
Type = "Executable"
Version = "1.1.4"
Source = [
    "Program.cs",
]
```

## Program.cs
A C# Program file that implements the main method.
```
using System;

namespace SimpleConsoleApplication
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
