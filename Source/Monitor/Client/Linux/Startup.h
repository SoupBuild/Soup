#pragma once

#include "LinuxConnectionManager.h"

extern "C"
{
	typedef int  (*open_ptr) (const char *path, int oflag, ...);
} 

class Startup
{
public:
	Monitor::LinuxConnectionManager connectionManager;
	open_ptr open;

public:
	Startup() :
		connectionManager()
	{
		try
		{
			auto traceProcessId = 123; // s_Payload.nTraceProcessId;

			// Extract the allowed read/write directories
			auto workingDirectory = Opal::Path(); // Opal::Path(s_Payload.zWorkingDirectory);
			bool enableAccessChecks = false; //s_Payload.EnableAccessChecks;
			auto allowedReadDirectories = std::vector<std::string>(); // ExtractStringList(s_Payload.zReadAccessDirectories, s_Payload.cReadAccessDirectories);
			auto allowedWriteDirectories = std::vector<std::string>(); // ExtractStringList(s_Payload.zWriteAccessDirectories, s_Payload.cWriteAccessDirectories);

			// Initialize the event pipe
			connectionManager.Initialize(traceProcessId);
			// Monitor::FileSystemAccessSandbox::Initialize(
			// 	enableAccessChecks,
			// 	std::move(workingDirectory),
			// 	std::move(allowedReadDirectories),
			// 	std::move(allowedWriteDirectories));

			AttachDetours();
		}
		catch (const std::exception& ex)
		{
			connectionManager.WriteError(ex.what());
			exit(-1234);
		}
		catch (...)
		{
			connectionManager.WriteError("Unknown error attaching detours");
			exit(-1234);
		}
	}

private:
	void AttachDetours()
	{
		open = (open_ptr)dlsym(RTLD_NEXT, "open");
	}
};

static Startup startup;

int open(const char *path, int oflag, ...)
{
	va_list args;
	va_start(args, oflag);
	va_end(args);
	return startup.open(path, oflag);
}