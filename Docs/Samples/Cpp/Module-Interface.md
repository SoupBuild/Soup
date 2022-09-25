# C++ Module Interface
A console application that uses a single module interface file used inside the same projects.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/ModuleInterface)

## Recipe.sml
The Recipe file that sets the name, type, version, the public interface module and the single source file.
```
Name: "Samples.Cpp.ModuleInterface"
Language: "C++|0.1"
Type: "Executable"
Version: "1.0.0"
Interface: "Module.cpp"
Source: [
    "Main.cpp"
]
```

## Module.cpp
A module interface file that exports a single sample class.
```
module;

// Include all standard library headers in the global module
#include <string>

export module Samples.Cpp.ModuleInterface;

export class Helper
{
public:
    static std::string GetName()
    {
        return "Soup";
    }
};
```

## Main.cpp
A simple main method that prints our "Hello World, Soup Style!" by using the module from the previous file.
```
#include <iostream>

import Samples.Cpp.ModuleInterface;

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