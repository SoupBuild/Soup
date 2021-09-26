# Static Library Reference
This is a console application that has a single static library dependency.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/CSharp/SimpleConsoleApplication)

## Recipe.toml
The Recipe file that defines the static library "MyLibrary".
```
Name = "Samples.SimpleConsoleApplication"
Language = "C#"
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
