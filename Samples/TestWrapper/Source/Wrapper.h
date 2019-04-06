#pragma once

export namespace TestWrapper_VersionNamespace
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
