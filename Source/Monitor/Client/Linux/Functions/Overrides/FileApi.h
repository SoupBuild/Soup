#pragma once

#include "../Cache/FileApi.h"

int open(const char* path, int oflag, ... /* mode_t mode */ )
{
	std::cout << "got open" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::open));

	// TODO: Bigly hack since Clang does not support __builtin_va_arg_pack
	// To whomever thought variadic optional parameters were a good idea. Why?
	bool requiresMode = (oflag & O_CREAT) != 0 || (oflag & __O_TMPFILE) == __O_TMPFILE;
	int result;
	if (requiresMode)
	{
		va_list args;
		va_start(args, oflag);
		auto mode = va_arg(args, mode_t);
		va_end(args);
		result = Monitor::Linux::Functions::Cache::FileApi::open(path, oflag, mode);
	}
	else
	{
		result = Monitor::Linux::Functions::Cache::FileApi::open(path, oflag);
	}

	message.AppendValue(path);
	message.AppendValue(oflag);
	message.AppendValue(result);

	return result;
}

int creat(const char *pathname, mode_t mode)
{
	std::cout << "got creat" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::creat));

	auto result = Monitor::Linux::Functions::Cache::FileApi::creat(pathname, mode);

	message.AppendValue(pathname);
	message.AppendValue(result);

	return result;
}

int openat(int dirfd, const char *pathname, int flags, ... /* mode_t mode */ )
{
	std::cout << "got openat" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::openat));

	// TODO: Bigly hack since Clang does not support __builtin_va_arg_pack
	// To whomever thought variadic optional parameters were a good idea. Why?
	bool requiresMode = (flags & O_CREAT) != 0 || (flags & __O_TMPFILE) == __O_TMPFILE;
	int result;
	if (requiresMode)
	{
		va_list args;
		va_start(args, flags);
		auto mode = va_arg(args, mode_t);
		va_end(args);
		result = Monitor::Linux::Functions::Cache::FileApi::openat(dirfd, pathname, flags, mode);
	}
	else
	{
		result = Monitor::Linux::Functions::Cache::FileApi::openat(dirfd, pathname, flags);
	}

	message.AppendValue(dirfd);
	message.AppendValue(pathname);
	message.AppendValue(flags);
	message.AppendValue(result);

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