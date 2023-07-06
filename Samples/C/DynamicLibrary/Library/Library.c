module;

// Include all standard library headers in the global module
#include <string>

export module Samples.Cpp.DynamicLibrary.Library;

// Note: The namespace does not have to match the module name
export namespace Samples::Cpp::DynamicLibrary::Library
{
	class Helper
	{
	public:
	#ifdef _WIN32
		__declspec(dllexport)
	#endif
		static std::string GetName()
		{
			return "Soup";
		}
	};
}
