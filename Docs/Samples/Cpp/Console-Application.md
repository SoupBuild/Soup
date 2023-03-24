#  C++ Console Application
This is the smallest amount of code to get a console application building using Soup.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/ConsoleApplication)

## Recipe.sml
The Recipe file that sets the name, type, version and the single source file.
```
Name: "Samples.Cpp.ConsoleApplication"
Language: "C++|0"
Type: "Executable"
Version: "1.0.0"
Source: [
    "Main.cpp"
]
```

## PackageLock.sml
The package lock that was generated to capture the unique dependencies required to build this project.
```
Version: 4
Closures: {
    Root: {
        "C++": [
            { Name: "Samples.Cpp.ConsoleApplication", Version: "./", Build: "Build0", Tool: "Tool0" }
        ]
    }
    Build0: {
        Wren: [
            { Name: "Soup.Cpp", Version: "0.7.0" }
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

## Main.cpp
A simple main method that prints our "Hello World, Soup Style!" and returns a success status.
```
#include <iostream>

int main()
{
    std::cout << "Hello World, Soup Style!" << std::endl;
    return 0;
}
```

## .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```