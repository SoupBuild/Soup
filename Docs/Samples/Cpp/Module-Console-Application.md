# Module Console Application
A console application that uses a single module interface file used inside the same projects.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/ModuleConsoleApplication)

## Recipe.toml
The Recipe file that sets the name, type, version, the public interface module and the single source file.
```
Name = "ModuleConsoleApplication"
Language = "C++"
Type = "Executable"
Version = "1.2.5"
Interface = "Module.cpp"
Source = [
    "Main.cpp"
]
```

## Module.cpp
A module interface file that exports a single sample class.
```
module;

// Include all standard library headers in the global module
#include <string>

export module ModuleConsoleApplication;

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

import ModuleConsoleApplication;

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