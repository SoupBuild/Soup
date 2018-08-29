#pragma once

namespace TestWrapper
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
