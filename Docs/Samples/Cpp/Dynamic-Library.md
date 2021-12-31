# C++ Dynamic Library
This is a console application that has a single dynamic library dependency.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/DynamicLibrary)

## Library/Recipe.toml
The Recipe file that defines the static library "Samples.Cpp.DynamicLibrary.Library".
```
Name = "Samples.Cpp.DynamicLibrary.Library"
Language = "C++"
Version = "1.0.0"
Interface = "Module.cpp"
```

## Library/Module.cpp
A module interface file that exports a single sample class.
```
module;

// Include all standard library headers in the global module
#include <string>

export module Samples.Cpp.DynamicLibrary.Library;

// Note: The namespace does not have to match the module name
export namespace Samples.Cpp.DynamicLibrary.Library
{
    class Helper
    {
    public:
        __declspec(dllexport) static std::string GetName()
        {
            return "Soup";
        }
    };
}
```

## Application/Recipe.toml
The Recipe file that defines the executable "Samples.Cpp.DynamicLibrary.Application".
```
Name = "Samples.Cpp.DynamicLibrary.Application"
Language = "C++"
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

## Application/Main.cpp
A simple main method that prints our "Hello World, Soup Style!" by using the module from the library.
```
#include <iostream>

import Samples.Cpp.DynamicLibrary.Library;
using namespace Samples.Cpp.DynamicLibrary.Library;

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
