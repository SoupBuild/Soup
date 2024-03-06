#pragma once

#include "../Cache/FileApi.h"
#include "../../../MessageSender.h"

namespace Monitor::Linux::Functions::Overrides::FileApi
{
	int open(const char* path, int oflag, ...)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::open));

		va_list args;
		va_start(args, oflag);
		va_end(args);
		auto result = Cache::FileApi::open(path, oflag);

		message.AppendValue(path);
		message.AppendValue(result);

		return result;
	}
}