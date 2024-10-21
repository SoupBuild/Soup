#pragma once

#include "../Cache/ProcessApi.h"

int system(const char *command)
{
	connectionManager.DebugTrace("system");
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::system));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::system(command);

	message.AppendValue(command);

	return result;
}

pid_t fork(void)
{
	connectionManager.DebugTrace("fork");
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::fork));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::fork();

	return result;
}

// int execl(const char *path, const char *arg, ...)
// {
//	connectionManager.DebugTrace("execl");
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
//	connectionManager.DebugTrace("execlp");
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
//	connectionManager.DebugTrace("execle");
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
	connectionManager.DebugTrace("execv");
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execv));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execv(path, argv);

	message.AppendValue(path);
	message.AppendValue(result);

	return result;
}

int execvp(const char *file, char *const argv[])
{
	connectionManager.DebugTrace("execvp");
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execvp));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execvp(file, argv);

	message.AppendValue(file);
	message.AppendValue(result);

	return result;
}

int execvpe(const char *file, char *const argv[], char *const envp[])
{
	connectionManager.DebugTrace("execvpe");
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execvpe));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execvpe(file, argv, envp);

	message.AppendValue(file);
	message.AppendValue(result);

	return result;
}

int execve(const char *pathname, char *const argv[], char *const envp[])
{
	connectionManager.DebugTrace("execve");
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execve));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execve(file, argv, envp);

	message.AppendValue(pathname);
	message.AppendValue(result);
}

int execveat(int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags)
{
	connectionManager.DebugTrace("execveat");
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::execveat));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::execveat(dirfd, pathname, argv, envp, flags);

	message.AppendValue(pathname);
	message.AppendValue(result);
}

int fexecve(int fd, char *const argv[], char *const envp[])
{
	connectionManager.DebugTrace("fexecve");
	auto message = Monitor::MessageSender(Monitor::MessageType::Detour);
	message.AppendValue(static_cast<uint32_t>(Monitor::Linux::DetourEventType::fexecve));

	auto result = Monitor::Linux::Functions::Cache::ProcessApi::fexecve(fd, argv, envp);

	message.AppendValue(result);
}