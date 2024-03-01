
#pragma once
#include "../ConnectionManager.h"
#include "Detours.h"

namespace Monitor
{
	class LinuxConnectionManager : public ConnectionManager
	{
	public:
		LinuxConnectionManager() :
		 	ConnectionManager(),
			pipeHandle()
		{
		}

	protected:
		virtual void Connect(int32_t traceProcessId)
		{
			DebugTrace("ConnectionManager::Connect");

			auto pipeName = std::string("/tmp/soupbuildfifo");
			pipeHandle = detours.open(pipeName.c_str(), O_WRONLY);
		}

		virtual void Disconnect()
		{
			DebugTrace("ConnectionManager::Disconnect");
			close(pipeHandle);
		}

		virtual void UnsafeWriteMessage(const Message& message)
		{
			DebugTrace("ConnectionManager::UnsafeWriteMessage");
			// Write the message
			size_t countBytesToWrite = message.ContentSize +
				sizeof(Message::Type) +
				sizeof(Message::ContentSize);
			auto countBytesWritten = write(
				pipeHandle,
				&message,
				countBytesToWrite);
			if (countBytesWritten == -1)
			{
				DebugError("DETOURS-CLIENT-ERROR: Failed write event logger");
				exit(-1234);
			}

			if (countBytesWritten != countBytesToWrite)
			{
				DebugError("Did not write the expected number of bytes");
				exit(-1234);
			}
		}

	private:
		int pipeHandle;
	};
}