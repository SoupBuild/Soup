#pragma once

namespace Monitor::Windows
{
	#ifdef SOUP_BUILD
	export
	#endif
	enum class DetourEventType : uint32_t
	{
		// FileApi
		open,
	};
}