// Include all standard library headers in the global module
#include <string>

#ifdef _WIN32
#include <combaseapi.h>
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#define CoTaskMemAlloc(p) malloc(p)
#define CoTaskMemFree(p) free(p)
#endif

import Soup.Core;

extern "C" 
{
	DLL_EXPORT const char* GetName()
	{
		auto value = std::string("Some test to return");

		auto result = (char*)CoTaskMemAlloc(value.size() + 1);
		value.copy(result, value.size());
		result[value.size()] = 0;
		return result;
	}
	
	DLL_EXPORT int AddStuff(int a, int b)
	{
		return a + b;
	}
}