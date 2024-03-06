#pragma once

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
			if (value != nullptr)
			{
				auto stringValue = std::string_view(value);
				auto startIndex = message.ContentSize;
				auto size = stringValue.size() + 1;
				message.ContentSize += static_cast<uint32_t>(size);
				if (message.ContentSize > sizeof(message.Content))
				{
					throw std::runtime_error("Message content too long for const char* value");
				}

				stringValue.copy(reinterpret_cast<char*>(message.Content + startIndex), stringValue.size());
				message.Content[message.ContentSize - 1] = 0;
			}
			else
			{
				AppendValue("");
			}
		}

		void AppendValue(const wchar_t* value)
		{
			if (value != nullptr)
			{
				auto stringValue = std::wstring_view(value);
				auto startIndex = message.ContentSize;
				auto size = 2 * (stringValue.size() + 1);
				message.ContentSize += static_cast<uint32_t>(size);
				if (message.ContentSize > sizeof(message.Content))
				{
					throw std::runtime_error("Message content too long for const wchar_t* value");
				}

				stringValue.copy(reinterpret_cast<wchar_t*>(message.Content + startIndex), stringValue.size());
				message.Content[message.ContentSize - 1] = 0;
				message.Content[message.ContentSize - 2] = 0;
			}
			else
			{
				AppendValue(L"");
			}
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
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(int);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for int value");
			}

			*reinterpret_cast<int*>(message.Content + startIndex) = value;
		}

		void AppendValue(long value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(long);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for long value");
			}

			*reinterpret_cast<long*>(message.Content + startIndex) = value;
		}

		void AppendValue(unsigned int value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(unsigned int);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for unsigned int value");
			}

			*reinterpret_cast<unsigned int*>(message.Content + startIndex) = value;
		}

		void AppendValue(unsigned long value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(unsigned long);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for unsigned long value");
			}

			*reinterpret_cast<unsigned long*>(message.Content + startIndex) = value;
		}

		void AppendValue(unsigned long long value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(unsigned long long);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for unsigned long long value");
			}

			*reinterpret_cast<unsigned long long*>(message.Content + startIndex) = value;
		}
	};
}