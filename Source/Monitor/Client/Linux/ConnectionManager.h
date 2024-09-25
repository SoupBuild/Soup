
#pragma once
#include "../ConnectionManagerBase.h"
#include "Functions/Cache/FileApi.h"

namespace Monitor::Linux
{
	class ConnectionManager : public ConnectionManagerBase
	{
	public:
		ConnectionManager() :
		 	ConnectionManagerBase(),
			pipeHandle()
		{
		}

	protected:
		virtual void Connect(int32_t traceProcessId)
		{
			DebugTrace("ConnectionManager::Connect");

			auto pipeName = std::string("/tmp/soupbuildfifo");
			pipeHandle = Functions::Cache::FileApi::open(pipeName.c_str(), O_WRONLY);
		}

		virtual void Disconnect()
		{
			DebugTrace("ConnectionManager::Disconnect");
			close(pipeHandle);
		}

		virtual bool TryUnsafeWriteMessage(const Message& message)
		{
			DebugTrace("ConnectionManager::TryUnsafeWriteMessage");

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
				DebugError("Failed write event logger");
				return false;
			}

			if (countBytesWritten != countBytesToWrite)
			{
				DebugError("Did not write the expected number of bytes");
				return false;
			}

			return true;
		}

	private:
		int pipeHandle;
	};
}

// Create a singleton
static Monitor::Linux::ConnectionManager connectionManager;