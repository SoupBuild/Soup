# C++ Header Library
This is a console application that has a single header library dependency.

[Source](https://github.com/soup-build/soup/tree/main/samples/cpp/header-library)

## Library/Recipe.sml
The Recipe file that defines the header library "Samples.Cpp.HeaderLibrary.Library".
```sml
Name: 'Samples.Cpp.HeaderLibrary.Library'
Language: (C++@0)
Version: 1.0.0
IncludePaths: [
  'public/'
]
PublicHeaders: [
  {
    Root: './'
    Files: [
      'Library.h'
    ]
  }
]
```

## Library/Library.h
The header file.
```cpp
#include <string>

namespace Samples::Cpp::HeaderLibrary::Library
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

## Application/Recipe.sml
The Recipe file that defines the executable "Samples.Cpp.HeaderLibrary.Application".
```sml
Name: 'Samples.Cpp.HeaderLibrary.Application'
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
      'Samples.Cpp.HeaderLibrary.Application': { Version: '../Application', Build: 'Build0', Tool: 'Tool0' }
      'Samples.Cpp.HeaderLibrary.Library': { Version: '../Library/', Build: 'Build0', Tool: 'Tool0' }
    }
  }
  Build0: {
    Wren: {
      'Soup|Cpp': { Version: 0.13.2 }
    }
  }
  Tool0: {
    'C++': {
      'mwasplund|copy': { Version: 1.1.0 }
      'mwasplund|mkdir': { Version: 1.1.0 }
    }
  }
}
```

## Application/Main.cpp
A simple main method that prints our "Hello World, Soup Style!" by using the header from the library.
```cpp
#include <iostream>
#include <Library.h>

using namespace Samples::Cpp::HeaderLibrary::Library;

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
