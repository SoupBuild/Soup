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
