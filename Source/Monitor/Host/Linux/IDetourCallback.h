#pragma once 

namespace Monitor::Linux
{
	class IDetourCallback
	{
	public:
		virtual void OnInitialize() = 0;
		virtual void OnShutdown() = 0;
		virtual void OnError(std::string_view message) = 0;

		// FileApi
		virtual void OnOpen() = 0;
	};
}
