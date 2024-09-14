# C++ Module Dynamic Library
This is a console application that has a single dynamic library dependency.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/ModuleDynamicLibrary)

## Library/Recipe.sml
The Recipe file that defines the static library "Samples.Cpp.ModuleDynamicLibrary.Library".
```sml
Name: 'Samples.Cpp.ModuleDynamicLibrary.Library'
Language: (C++@0)
Version: 1.0.0
Interface: 'Module.cpp'
Type: 'DynamicLibrary'
```

## Library/Module.cpp
A module interface file that exports a single sample class.
```cpp
module;

// Include all standard library headers in the global module
#include <string>

export module Samples.Cpp.DynamicLibrary.Library;

// Note: The namespace does not have to match the module name
export namespace Samples::Cpp::DynamicLibrary::Library
{
  class Helper
  {
  public:
  #ifdef _WIN32
    __declspec(dllexport)
  #endif
    static std::string GetName()
    {
      return "Soup";
    }
  };
}
```

## Application/Recipe.sml
The Recipe file that defines the executable "Samples.Cpp.ModuleDynamicLibrary.Application".
```sml
Name: 'Samples.Cpp.ModuleDynamicLibrary.Application'
Language: (C++@0)
Type: 'Executable'
Version: 1.0.0
Source: [
  'Main.cpp'
]

Dependencies: {
  Runtime: [
    '../Library/'
  ]
}
```

## Application/PackageLock.sml
The package lock that was generated to capture the unique dependencies required to build this project and the dynamic library dependency.
```sml
Version: 5
Closures: {
  Root: {
    'C++': {
      'Samples.Cpp.ModuleDynamicLibrary.Application': { Version: '../Application', Build: 'Build0', Tool: 'Tool0' }
      'Samples.Cpp.ModuleDynamicLibrary.Library': { Version: '../Library/', Build: 'Build0', Tool: 'Tool0' }
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

## Application/Main.cpp
A simple main method that prints our "Hello World, Soup Style!" by using the module from the library.
```cpp
#include <iostream>

import Samples.Cpp.DynamicLibrary.Library;
using namespace Samples::Cpp::DynamicLibrary::Library;

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
