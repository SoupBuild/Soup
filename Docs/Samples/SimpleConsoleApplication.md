#  Simple Console Application
This is the smallest amount of code to get a console application building using Soup.

## Recipe.toml
The Recipe file that sets the name, type, version and the single source file.
```
Name = "SimpleConsoleApplication"
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