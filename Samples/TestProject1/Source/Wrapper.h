#pragma once
#include <string>

namespace TestProject1
{
SOUP_PKG_ACTIVE namespace SOUP_PKG_VERSION
{
	class Wrapper
	{
		public:
			Wrapper();
			int GetValue();
			std::wstring GetVersion();

		private:
			int _value;
	};
}
}