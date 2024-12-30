#pragma once
#include "MessageBuilder.h"

namespace Monitor
{
	/// RAI class to help with sending messages even when the internal message throws
	class MessageSender
	{
	private:
		Message message;

	public:
		MessageSender(MessageType type)
		{
			message.Type = type;
			message.ContentSize = 0;
		}

		~MessageSender()
		{
			connectionManager.WriteMessage(message);
		}

		void AppendValue(const char* value)
		{
			MessageBuilder::AppendValue(message, value);
		}

		void AppendValue(const wchar_t* value)
		{
			MessageBuilder::AppendValue(message, value);
		}

		void AppendValue(void* value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(void*);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for void* value");
			}

			*reinterpret_cast<void**>(message.Content + startIndex) = value;
		}

		void AppendValue(int value)
		{
			MessageBuilder::AppendValue(message, value);
		}

		void AppendValue(long value)
		{
			MessageBuilder::AppendValue(message, value);
		}

		void AppendValue(unsigned int value)
		{
			MessageBuilder::AppendValue(message, value);
		}

		void AppendValue(unsigned long value)
		{
			MessageBuilder::AppendValue(message, value);
		}

		void AppendValue(unsigned long long value)
		{
			MessageBuilder::AppendValue(message, value);
		}
	};
}