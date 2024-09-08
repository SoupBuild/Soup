
#pragma once
#include "Helpers.h"
#include "MessageBuilder.h"

namespace Monitor
{
	class ConnectionManagerBase
	{
	private:
		std::mutex pipeMutex;
		bool hadError;

	public:
		ConnectionManagerBase() :
			pipeMutex(),
			hadError(false)
		{
			DebugTrace("ConnectionManagerBase::ConnectionManagerBase");
		}

		void Initialize(int32_t traceProcessId)
		{
			DebugTrace("ConnectionManagerBase::Initialize");
			{
				auto lock = std::lock_guard<std::mutex>(pipeMutex);
				Connect(traceProcessId);
			}

			// Notify that we are connected
			Message message;
			message.Type = MessageType::Initialize;
			message.ContentSize = 0;
			WriteMessage(message);
		}

		void Shutdown()
		{
			DebugTrace("ConnectionManagerBase::Shutdown");
			auto lock = std::lock_guard<std::mutex>(pipeMutex);
			Message message;
			message.Type = MessageType::Shutdown;
			message.ContentSize = 0;
			MessageBuilder::AppendValue(message, hadError);
			if (!TryUnsafeWriteMessage(message))
			{
				// Not much we can do at the end...
				hadError = true;
			}

			Disconnect();
		}

		void WriteMessage(const Message& message)
		{
			auto lock = std::lock_guard<std::mutex>(pipeMutex);
			if (!TryUnsafeWriteMessage(message))
			{
				// Save the failure for the final response
				hadError = true;
			}
		}

		void DebugError(std::string_view message, uint32_t value)
		{
			#ifdef TRACE_DETOUR_CLIENT
			printf("DETOUR-CLIENT-ERROR: %s %u\n", message.data(), value);
			#else
			(message);
			(value);
			#endif
		}

		void DebugError(std::string_view message)
		{
			#ifdef TRACE_DETOUR_CLIENT
			printf("DETOUR-CLIENT-ERROR: %s\n", message.data());
			#else
			(message);
			#endif
		}

	#ifdef TRACE_DETOUR_CLIENT
		template<typename... Args>
		static void DebugTrace(std::string_view message, Args&&... args)
		{
			auto result = std::vformat(message, std::make_format_args(args...))
			printf("DETOUR-CLIENT: %s\n", result.c_str());
		}
	#else
		template<typename... Args>
		static void DebugTrace(std::string_view, Args&&...)
		{
			// NO-OP
		}
	#endif

	protected:
		virtual void Connect(int32_t traceProcessId) = 0;
		virtual void Disconnect() = 0;
		virtual bool TryUnsafeWriteMessage(const Message& message) = 0;
	};
}