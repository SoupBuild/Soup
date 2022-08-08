# Static Library Reference
This is a console application that has a single static library dependency.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/StaticLibrary)

## Library/Recipe.toml
The Recipe file that defines the static library "Samples.Cpp.StaticLibrary.Library".
```
Name = "Samples.Cpp.StaticLibrary.Library"
Language = "C++|0.1"
Version = "1.0.0"
Interface = "Module.cpp"
```

## Library/Module.cpp
A module interface file that exports a single sample class.
```
module;

// Include all standard library headers in the global module
#include <string>

export module Samples.Cpp.StaticLibrary.Library;

// Note: The namespace does not have to match the module name
export namespace Samples.Cpp.StaticLibrary.Library
{
    class Helper
    {
    public:
        static std::string GetName()
        {
            return "Soup";
        }
    };
}
```

## Application/Recipe.toml
The Recipe file that defines the executable "Samples.Cpp.StaticLibrary.Application".
```
Name = "Samples.Cpp.StaticLibrary.Application"
Language = "C++|0.1"
Type = "Executable"
Version = "1.0.0"
Source = [
    "Main.cpp"
]

[Dependencies]
Runtime = [
    "../Library/"
]
```

## MyApplication/Main.cpp
A simple main method that prints our "Hello World, Soup Style!" by using the module from the library.
```
#include <iostream>

import Samples.Cpp.StaticLibrary.Library;
using namespace Samples.Cpp.StaticLibrary.Library;

int main()
{
    std::cout << "Hello World, " << Helper::GetName() << " Style!" << std::endl;
    return 0;
}
```

## .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```
