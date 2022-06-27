#pragma once
#include "MessageType.h"

namespace Monitor
{
	export class Message
	{
	public:
		MessageType Type;
		uint32_t ContentSize;
		uint8_t Content[2048 - sizeof(Type) - sizeof(ContentSize)];

	public:
		void AppendValue(const char* value)
		{
			if (value != nullptr)
			{
				auto stringValue = std::string_view(value);
				auto startIndex = ContentSize;
				auto size = stringValue.size() + 1;
				ContentSize += static_cast<uint32_t>(size);
				if (ContentSize > sizeof(Content))
				{
					throw std::runtime_error("Message content too long for const char* value");
				}

				stringValue.copy(reinterpret_cast<char*>(Content + startIndex), size);
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
				auto startIndex = ContentSize;
				auto size = 2 * (stringValue.size() + 1);
				ContentSize += static_cast<uint32_t>(size);
				if (ContentSize > sizeof(Content))
				{
					throw std::runtime_error("Message content too long for const wchar_t* value");
				}

				stringValue.copy(reinterpret_cast<wchar_t*>(Content + startIndex), size);
			}
			else
			{
				AppendValue(L"");
			}
		}

		void AppendValue(void* value)
		{
			auto startIndex = ContentSize;
			ContentSize += sizeof(void*);
			if (ContentSize > sizeof(Content))
			{
				throw std::runtime_error("Message content too long for void* value");
			}

			*reinterpret_cast<void**>(Content + startIndex) = value;
		}

		void AppendValue(int value)
		{
			auto startIndex = ContentSize;
			ContentSize += sizeof(int);
			if (ContentSize > sizeof(Content))
			{
				throw std::runtime_error("Message content too long for int value");
			}

			*reinterpret_cast<int*>(Content + startIndex) = value;
		}

		void AppendValue(long value)
		{
			auto startIndex = ContentSize;
			ContentSize += sizeof(long);
			if (ContentSize > sizeof(Content))
			{
				throw std::runtime_error("Message content too long for long value");
			}

			*reinterpret_cast<long*>(Content + startIndex) = value;
		}

		void AppendValue(unsigned int value)
		{
			auto startIndex = ContentSize;
			ContentSize += sizeof(unsigned int);
			if (ContentSize > sizeof(Content))
			{
				throw std::runtime_error("Message content too long for unsigned int value");
			}

			*reinterpret_cast<unsigned int*>(Content + startIndex) = value;
		}

		void AppendValue(unsigned long value)
		{
			auto startIndex = ContentSize;
			ContentSize += sizeof(unsigned long);
			if (ContentSize > sizeof(Content))
			{
				throw std::runtime_error("Message content too long for unsigned long value");
			}

			*reinterpret_cast<unsigned long*>(Content + startIndex) = value;
		}

		void AppendValue(unsigned long long value)
		{
			auto startIndex = ContentSize;
			ContentSize += sizeof(unsigned long long);
			if (ContentSize > sizeof(Content))
			{
				throw std::runtime_error("Message content too long for unsigned long long value");
			}

			*reinterpret_cast<unsigned long long*>(Content + startIndex) = value;
		}
	};
}