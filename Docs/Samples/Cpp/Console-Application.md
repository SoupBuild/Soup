#  C++ Console Application
This is the smallest amount of code to get a console application building using Soup.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/ConsoleApplication)

## Recipe.toml
The Recipe file that sets the name, type, version and the single source file.
```
Name = "Samples.Cpp.ConsoleApplication"
Language = "C++|0.1"
Type = "Executable"
Version = "1.1.3"
Source = [
    "Main.cpp"
]
```

## Main.cpp
A simple main method that prints our "Hello World, Soup Style!" and returns a success status.
```
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