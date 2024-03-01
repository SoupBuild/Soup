#pragma once

#include "..\Cache\FileApis.h"

namespace Monitor::Linux::Functions::Overrides::FileApi
{
	int open(const char* path, int oflag, ...)
	{
		int result = 0;
		__try
		{
			va_list args;
			va_start(args, oflag);
			va_end(args);
			result = Cache::FileApi::open(path, oflag);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::open));
			message.AppendValue(path);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}
}