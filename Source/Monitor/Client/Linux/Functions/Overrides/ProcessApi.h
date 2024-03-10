#pragma once

#include "../Cache/ProcessApi.h"

int system(const char *command)
{
	std::cout << "got system" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::system));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::system(command);

	message.AppendValue(command);

	return result;
}

pid_t fork(void)
{
	std::cout << "got fork" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::fork));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::fork();

	return result;
}

// int execl(const char *path, const char *arg, ...)
// {
// 	std::cout << "got execl" << std::endl;
// 	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
// 	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execl));

// 	va_list args;
// 	va_start(args, arg);
// 	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execl(path, arg, args);
// 	va_end(args);

// 	message.AppendValue(path);
// 	message.AppendValue(result);

// 	return result;
// }

// int execlp(const char *file, const char *arg, ...)
// {
// 	std::cout << "got execlp" << std::endl;
// 	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
// 	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execlp));

// 	va_list args;
// 	va_start(args, arg);
// 	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execlp(file, arg, args);
// 	va_end(args);

// 	message.AppendValue(file);
// 	message.AppendValue(result);

// 	return result;
// }

// int execle(const char *path, const char *arg, ..., char * const envp[])
// {
//	std::cout << "got execle" << std::endl;
// 	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
// 	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execle));

// 	va_list args;
// 	va_start(args, arg);
// 	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execle(path, arg, args, envp);
// 	va_end(args);

// 	message.AppendValue(path);
// 	message.AppendValue(result);

// 	return result;
// }

int execv(const char *path, char *const argv[])
{
	std::cout << "got execv" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execv));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execv(path, argv);

	message.AppendValue(path);
	message.AppendValue(result);

	return result;
}

int execvp(const char *file, char *const argv[])
{
	std::cout << "got execvp" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execvp));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execvp(file, argv);

	message.AppendValue(file);
	message.AppendValue(result);

	return result;
}

int execvpe(const char *file, char *const argv[], char *const envp[])
{
	std::cout << "got execvpe" << std::endl;
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execvpe));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execvpe(file, argv, envp);

	message.AppendValue(file);
	message.AppendValue(result);

	return result;
}