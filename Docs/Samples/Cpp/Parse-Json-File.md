#  Parse Json File
A console application that reads in a json file using the an external module and prints a single known property value.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/ParseJsonFile)

## Recipe.sml
The Recipe file that sets the standard name, type, version, as well as the single external dependency of the [json11](https://github.com/dropbox/json11) project.
```
Name: "Samples.Cpp.ParseJsonFile"
Language: "C++|0.1"
Version: "1.0.0"
Type: "Executable"
Source: [
    "Main.cpp"
]

Dependencies: {
    Runtime: [
        "json11@1.0.2"
    ]
}
```

## Message.json
A json file containing a single property containing a message for the application to print.
```
{
  "message": "Hello!"
}
```

## Main.cpp
A simple main method that reads the contents of a single json file, parses the json content and prints a single message from a known property.
```
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

import json11;

int main()
{
    // Read in the contents of the json file
    auto jsonFile = std::ifstream("Message.json");
    auto jsonContent = std::string(
        std::istreambuf_iterator<char>(jsonFile),
        std::istreambuf_iterator<char>());

    // Parse the json
    std::string errorMessage;
    auto json = json11::Json::parse(jsonContent, errorMessage);

    // Print the single property value
    std::cout << "Message: " << json["message"].string_value() << std::endl;

    return 0;
}
```

## .gitignore
A simple git ignore file to exclude all Soup build output.
```
out/
```