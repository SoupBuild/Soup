module;

// Include all standard library headers in the global module
#include <string>

export module Samples.Cpp.ModuleInterface;

export class Helper
{
public:
	static std::string GetName()
	{
		return "Soup";
	}
};
