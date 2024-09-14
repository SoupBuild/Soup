# C++ Module Interface
A console application that uses a single module interface file used inside the same projects.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/ModuleInterface)

## Recipe.sml
The Recipe file that sets the name, type, version, the public interface module and the single source file.
```sml
Name: 'Samples.Cpp.ModuleInterface'
Language: (C++@0)
Type: 'Executable'
Version: 1.0.0
Interface: 'Module.cpp'
Source: [
  'Main.cpp'
]
```

## Executable/PackageLock.sml
The package lock that was generated to capture the unique build dependencies required to build this project.
```sml
Version: 5
Closures: {
  Root: {
    'C++': {
      'Samples.Cpp.ModuleInterface': { Version: '../ModuleInterface', Build: 'Build0', Tool: 'Tool0' }
    }
  }
  Build0: {
    Wren: {
      'mwasplund|Soup.Cpp': { Version: '0.12.0' }
    }
  }
  Tool0: {
    'C++': {
      'mwasplund|copy': { Version: '1.1.0' }
      'mwasplund|mkdir': { Version: '1.1.0' }
    }
  }
}
```

## Module.cpp
A module interface file that exports a single sample class.
```cpp
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
```cpp
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