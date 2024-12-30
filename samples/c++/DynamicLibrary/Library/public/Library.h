#ifdef _WIN32
	#ifdef EXPORT_LIBRARY
		#define LIBRARY_API __declspec(dllexport)
	#else
		#define LIBRARY_API __declspec(dllimport)
	#endif
#else
	#define LIBRARY_API
#endif

namespace Samples::Cpp::DynamicLibrary::Library
{
	class Helper
	{
	public:
		LIBRARY_API static const char* GetName();
	};
}
