#pragma once

namespace Monitor
{
	#ifdef SOUP_BUILD
	export
	#endif
	enum class MessageType : uint32_t
	{
		Initialize,
		Shutdown,
		Error,

		Detour,
	};
}