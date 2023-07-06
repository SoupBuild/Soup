# C++ Dynamic Library
This is a console application that has a single dynamic library dependency.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/C/DynamicLibrary)

## Library/Recipe.sml
The Recipe file that defines the static library "Samples.C.DynamicLibrary.Library".
```
Name: "Samples.C.DynamicLibrary.Library"
Language: "C|0.1"
Version: "1.0.0"
Type: "DynamicLibrary"
Source: [
	"Library.c"
]
PublicHeaders: [
	"Library.h"
]
```

## Library/Library.h
A header interface file that declares a single sample function.
```
#ifdef _WIN32
__declspec(dllexport)
#endif
const char* GetName();
```

## Library/Library.c
A implementation file that defines a single sample function.
```
#include "Library.h"

const char* GetName()
{
	return "Soup";
}
```

## Application/Recipe.sml
The Recipe file that defines the executable "Samples.C.DynamicLibrary.Application".
```
Name: "Samples.C.DynamicLibrary.Application"
Language: "C|0.1"
Type: "Executable"
Version: "1.0.0"
Source: [
	"Main.c"
]

Dependencies: {
	Runtime: [
		"../Library/"
	]
}
```

## Application/PackageLock.sml
The package lock that was generated to capture the unique dependencies required to build this project and the dynamic library dependency.
```
Version: 4
Closures: {
	Root: {
		C: [
			{ Name: "Samples.C.DynamicLibrary.Application", Version: "../Application", Build: "Build0", Tool: "Tool0" }
			{ Name: "Samples.C.DynamicLibrary.Library", Version: "../Library/", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Soup.C", Version: "0.1.0" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}
```

## Application/Main.c
A simple main method that prints our "Hello World, Soup Style!" by using the module from the library.
```
#include <stdio.h>
#include <Library.h>

int main()
{
	printf("Hello World, %s Style!", GetName());
	return 0;
}
```

## .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```
