#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

import json11;

int main()
{
	// Read in the contents of the json file
	auto jsonFile = std::ifstream("./Message.json");
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