#pragma once

#include "ConnectionManager.h"
#include "AttachDetours.h"

using namespace Monitor::Linux;

class Startup
{
public:
	Startup() :
		connectionManager()
	{
		try
		{
			DebugTrace("Startup");
			size_t traceProcessId = 123; // s_Payload.nTraceProcessId;

			// Extract the allowed read/write directories
			auto workingDirectory = Opal::Path(); // Opal::Path(s_Payload.zWorkingDirectory);
			bool enableAccessChecks = false; //s_Payload.EnableAccessChecks;
			auto allowedReadDirectories = std::vector<std::string>(); // ExtractStringList(s_Payload.zReadAccessDirectories, s_Payload.cReadAccessDirectories);
			auto allowedWriteDirectories = std::vector<std::string>(); // ExtractStringList(s_Payload.zWriteAccessDirectories, s_Payload.cWriteAccessDirectories);

			AttachDetours();

			// Initialize the event pipe
			DebugTrace("ConnectionManager");
			connectionManager.Initialize(traceProcessId);
			// Monitor::FileSystemAccessSandbox::Initialize(
			// 	enableAccessChecks,
			// 	std::move(workingDirectory),
			// 	std::move(allowedReadDirectories),
			// 	std::move(allowedWriteDirectories));
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
	void DebugTrace(std::string_view message)
	{
//#ifdef TRACE_MONITOR_HOST
		std::cout << "Startup: " << message << std::endl;
// #else
// 		(message);
// #endif
	}
};

static Startup startup;