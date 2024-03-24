# Static Library Reference
This is a console application that has a single static library dependency.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/StaticLibrary)

## Library/Recipe.sml
The Recipe file that defines the static library "Samples.Cpp.StaticLibrary.Library".
```sml
Name: 'Samples.Cpp.StaticLibrary.Library'
Language: 'C++|0'
Version: '1.0.0'
Interface: 'Module.cpp'
Type: 'StaticLibrary'
```

## Library/Module.cpp
A module interface file that exports a single sample class.
```cpp
module;

// Include all standard library headers in the global module
#include <string>

export module Samples.Cpp.StaticLibrary.Library;

// Note: The namespace does not have to match the module name
export namespace Samples::Cpp::StaticLibrary::Library
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
The Recipe file that defines the executable "Samples.Cpp.StaticLibrary.Application".
```sml
Name: 'Samples.Cpp.StaticLibrary.Application'
Language: 'C++|0'
Type: 'Executable'
Version: '1.0.0'
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
The package lock that was generated to capture the unique dependencies required to build this project and the dependency static library.
```sml
Version: 5
Closures: {
	Root: {
		'C++': {
			'Samples.Cpp.StaticLibrary.Application': { Version: '../Application', Build: 'Build0', Tool: 'Tool0' }
			'Samples.Cpp.StaticLibrary.Library': { Version: '../Library/', Build: 'Build0', Tool: 'Tool0' }
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

## MyApplication/Main.cpp
A simple main method that prints our "Hello World, Soup Style!" by using the module from the library.
```cpp
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
