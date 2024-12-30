#pragma once

namespace Monitor
{
	class MessageBuilder
	{
	public:
		static void AppendValue(Message& message, const char* value)
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
				AppendValue(message, "");
			}
		}

		static void AppendValue(Message& message, const wchar_t* value)
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
				AppendValue(message, L"");
			}
		}

		static void AppendValue(Message& message, void* value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(void*);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for void* value");
			}

			*reinterpret_cast<void**>(message.Content + startIndex) = value;
		}

		static void AppendValue(Message& message, int value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(int);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for int value");
			}

			*reinterpret_cast<int*>(message.Content + startIndex) = value;
		}

		static void AppendValue(Message& message, long value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(long);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for long value");
			}

			*reinterpret_cast<long*>(message.Content + startIndex) = value;
		}

		static void AppendValue(Message& message, unsigned int value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(unsigned int);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for unsigned int value");
			}

			*reinterpret_cast<unsigned int*>(message.Content + startIndex) = value;
		}

		static void AppendValue(Message& message, unsigned long value)
		{
			auto startIndex = message.ContentSize;
			message.ContentSize += sizeof(unsigned long);
			if (message.ContentSize > sizeof(message.Content))
			{
				throw std::runtime_error("Message content too long for unsigned long value");
			}

			*reinterpret_cast<unsigned long*>(message.Content + startIndex) = value;
		}

		static void AppendValue(Message& message, unsigned long long value)
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