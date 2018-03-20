#pragma once
#include <string>

namespace TestProject1
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