#pragma once

namespace TestWrapper::SoupLatest
{
	class Wrapper
	{
		public:
			Wrapper();
			int GetValue();
			int GetVersion();

		private:
			int _value;
	};
}
