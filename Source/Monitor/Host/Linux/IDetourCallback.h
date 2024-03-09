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
		virtual void OnOpen(std::string_view path, int32_t oflag, int32_t result) = 0;

		virtual void OnFOpen(std::string_view pathname, std::string_view mode) = 0;
		virtual void OnFDOpen(int32_t fd, std::string_view mode) = 0;
		virtual void OnFReopen(std::string_view pathname, std::string_view mode) = 0;

		virtual void OnMkdir(std::string_view path, std::string_view mode) = 0;
	};
}
