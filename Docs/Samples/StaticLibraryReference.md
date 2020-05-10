# Static Library Reference
This is a console application that has a single static library dependency.

## MyLibrary/Recipe.toml
The Recipe file that defines the static library "MyLibrary".
```
Name = "MyLibrary"
Version = "1.0.0"
Public = "Module.cpp"
```

## MyLibrary/Module.cpp
A module interface file that exports a single sample class.
```
module;

// Include all standard library headers in the global module
#include <string>

export module MyLibrary;

export namespace MyLibrary
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

## MyApplication/Recipe.toml
The Recipe file that defines the executable "MyApplication".
```
Name = "MyApplication"
Type = "Executable"
Version = "1.0.0"
Dependencies = [
    "../MyLibrary/"
]
Source = [
    "Main.cpp"
]
```

## MyApplication/Main.cpp
A simple main method that prints our "Hello World, Soup Style!" by using the module from the library.
```
#include <iostream>

import MyLibrary;
using namespace MyLibrary;

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
