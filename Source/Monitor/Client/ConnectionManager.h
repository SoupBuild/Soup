
#pragma once
#include "Helpers.h"

namespace Monitor
{
	class ConnectionManager
	{
	private:
		std::mutex pipeMutex;

	public:
		ConnectionManager() :
			pipeMutex()
		{
			DebugTrace("ConnectionManager::ConnectionManager");
		}

		void Initialize(int32_t traceProcessId)
		{
			DebugTrace("ConnectionManager::Initialize");
			{
				auto lock = std::lock_guard<std::mutex>(pipeMutex);
				Connect(traceProcessId);
			}

			// Notify that we are connected
			Monitor::Message message;
			message.Type = Monitor::MessageType::Initialize;
			message.ContentSize = 0;
			WriteMessage(message);
		}

		void Shutdown()
		{
			DebugTrace("ConnectionManager::Shutdown");
			auto lock = std::lock_guard<std::mutex>(pipeMutex);
			Monitor::Message message;
			message.Type = Monitor::MessageType::Shutdown;
			message.ContentSize = 0;
			UnsafeWriteMessage(message);

			Disconnect();
		}

		void WriteError(std::string_view value)
		{
			DebugError(value.data());

			Monitor::Message message;
			message.Type = Monitor::MessageType::Error;
			message.ContentSize = 0;
			message.AppendValue(value.data());
			WriteMessage(message);
		}

		void WriteMessage(const Monitor::Message& message)
		{
			auto lock = std::lock_guard<std::mutex>(pipeMutex);
			UnsafeWriteMessage(message);
		}

		void DebugError(std::string_view message, uint32_t value)
		{
			printf("DETOUR-CLIENT-ERROR: %s %u\n", message.data(), value);
		}

		void DebugError(std::string_view message)
		{
			printf("DETOUR-CLIENT-ERROR: %s\n", message.data());
		}

		void DebugTrace(std::string_view message)
		{
	#ifdef TRACE_DETOUR_CLIENT
			printf("DETOUR-CLIENT: %s\n", message.data());
	#else
			(message);
	#endif
		}

	protected:
		virtual void Connect(int32_t traceProcessId) = 0;
		virtual void Disconnect() = 0;
		virtual void UnsafeWriteMessage(const Monitor::Message& message) = 0;
	};
}