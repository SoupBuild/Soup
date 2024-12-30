#pragma once
#include "MessageType.h"

namespace Monitor
{
	#ifdef SOUP_BUILD
	export
	#endif
	struct Message
	{
	public:
		MessageType Type;
		uint32_t ContentSize;
		uint8_t Content[2048 - sizeof(Type) - sizeof(ContentSize)];
	};
}