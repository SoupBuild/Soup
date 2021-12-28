module;

// Include all standard library headers in the global module
#include <string>

export module Samples.Cpp.DynamicLibrary.Library;

// Note: The namespace does not have to match the module name
export namespace Samples.Cpp.DynamicLibrary.Library
{
	class Helper
	{
	public:
		__declspec(dllexport) static std::string GetName()
		{
			return "Soup";
		}
	};
}
