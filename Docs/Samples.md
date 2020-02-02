# Samples
These samples are a small set of all the cool things you can do with the Soup Build System. The source for these samples can be found in the main Soup repository in the Samples folder.

## Simple Console Application

This is the smallest amount of code to get a console application building using Soup.

### Main.cpp
A simple main method that prints our "Hello World, Soup Style!" and returns a success status.
```
#include <iostream>

int main()
{
	std::cout << "Hello World, Soup Style!" << std::endl;
	return 0;
}
```

### Recipe.toml
The Recipe file that sets the name, type, version and the single source file.
```
Name = "SimpleConsoleApplication"
Type = "Executable"
Version = "1.1.3"
Source = [
	"Main.cpp"
]

```

### .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```

## Module Console Application

This is a console application that uses a single module interface file used inside the same projects.

### Module.cpp
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

### Main.cpp
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

### Recipe.toml
The Recipe file that sets the name, type, version, the public interface module and the single source file.
```
Name = "ModuleConsoleApplication"
Type = "Executable"
Version = "1.2.5"
Public = "Module.cpp"
Source = [
	"Main.cpp"
]
```

### .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```