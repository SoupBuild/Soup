# Static Library Reference
This is a console application that has a single static library dependency.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/C/StaticLibrary)

## Library/Recipe.sml
The Recipe file that defines the static library "Samples.C.StaticLibrary.Library".
```
Name: "Samples.C.StaticLibrary.Library"
Language: "C|0.1"
Version: "1.0.0"
Type: "StaticLibrary"
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
The Recipe file that defines the executable "Samples.C.StaticLibrary.Application".
```
Name: "Samples.C.StaticLibrary.Application"
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
The package lock that was generated to capture the unique dependencies required to build this project and the dependency static library.
```
Version: 4
Closures: {
	Root: {
		C: [
			{ Name: "Samples.C.StaticLibrary.Application", Version: "../Application", Build: "Build0", Tool: "Tool0" }
			{ Name: "Samples.C.StaticLibrary.Library", Version: "../Library/", Build: "Build0", Tool: "Tool0" }
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

## MyApplication/Main.c
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
