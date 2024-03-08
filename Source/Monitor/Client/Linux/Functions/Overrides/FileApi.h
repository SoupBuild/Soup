#pragma once

#include "../Cache/FileApi.h"

int open(const char* path, int oflag, ...)
{
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::open));

	va_list args;
	va_start(args, oflag);
	va_end(args);
	auto result = Monitor::Linux::Functions::Cache::FileApi::open(path, oflag);

	message.AppendValue(path);

	return result;
}

FILE* fopen(const char * pathname, const char * mode)
{
	std::cout << "got fopen" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::fopen));

	auto result = Monitor::Linux::Functions::Cache::FileApi::fopen(pathname, mode);

	message.AppendValue(pathname);
	message.AppendValue(mode);

	return result;
}

FILE* fdopen(int fd, const char *mode)
{
	std::cout << "got fdopen" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::fdopen));

	auto result = Monitor::Linux::Functions::Cache::FileApi::fdopen(fd, mode);

	message.AppendValue(fd);
	message.AppendValue(mode);

	return result;
}

FILE* freopen(
	const char * pathname,
	const char * mode,
	FILE*  stream)
{
	std::cout << "got freopen" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::freopen));

	auto result = Monitor::Linux::Functions::Cache::FileApi::freopen(pathname, mode, stream);

	message.AppendValue(pathname);
	message.AppendValue(mode);

	return result;
}

int mkdir(const char *path, mode_t mode)
{
	std::cout << "got mkdir" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::mkdir));

	auto result = Monitor::Linux::Functions::Cache::FileApi::mkdir(path, mode);

	message.AppendValue(path);
	message.AppendValue(mode);

	return result;
}