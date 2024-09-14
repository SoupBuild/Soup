#  C++ Console Application
This is the smallest amount of code to get a console application building using Soup.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/ConsoleApplication)

## Recipe.sml
The Recipe file that sets the name, type, version and the single source file.
```sml
Name: 'Samples.Cpp.ConsoleApplication'
Language: (C++@0)
Type: 'Executable'
Version: 1.0.0
Source: [
  'Main.cpp'
]
```

## PackageLock.sml
The package lock that was generated to capture the unique dependencies required to build this project.
```sml
Version: 5
Closures: {
  Root: {
    'C++': {
      'Samples.Cpp.ConsoleApplication': { Version: '../ConsoleApplication', Build: 'Build0', Tool: 'Tool0' }
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

## Main.cpp
A simple main method that prints our "Hello World, Soup Style!" and returns a success status.
```cpp
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