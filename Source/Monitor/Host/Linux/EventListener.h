// <copyright file="EventListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IDetourCallback.h"

namespace Monitor::Linux
{
	/// <summary>
	/// The event listener knows how to parse an incoming message and pass it along to the
	/// registered callback.
	/// </summary>
	class EventListener
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='EventListener'/> class.
		/// </summary>
		EventListener(
			std::shared_ptr<IDetourCallback> callback) :
			m_callback(std::move(callback))
		{
		}

		void LogError(std::string_view message)
		{
			m_callback->OnError(message);
		}

		void LogMessage(Message& message)
		{
			uint32_t offset = 0;
			switch (message.Type)
			{
				// Info
				case MessageType::Initialize:
				{
					m_callback->OnInitialize();
					break;
				}
				case MessageType::Shutdown:
				{
					m_callback->OnShutdown();
					break;
				}
				case MessageType::Error:
				{
					auto errorMessage = ReadStringValue(message, offset);
					m_callback->OnError(errorMessage);
					break;
				}
				case MessageType::Detour:
				{
					HandleDetourMessage(message, offset);
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown message type");
				}
			}

			// Verify that we read the entire message
			if (offset != message.ContentSize)
			{
				throw std::runtime_error("Did not read the entire message");
			}
		}

	private:
		void HandleDetourMessage(Message& message, uint32_t& offset)
		{
			auto eventType = static_cast<DetourEventType>(ReadUInt32Value(message, offset));
			switch (eventType)
			{
				// FileApi
				case DetourEventType::open:
				{
					auto path = ReadStringValue(message, offset);
					auto oflag = ReadInt32Value(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnOpen(path, oflag, result);
					break;
				}
				case DetourEventType::creat:
				{
					auto path = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnCreat(path, result);
					break;
				}
				case DetourEventType::openat:
				{
					auto dirfd = ReadInt32Value(message, offset);
					auto path = ReadStringValue(message, offset);
					auto oflag = ReadInt32Value(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnOpenat(dirfd, path, oflag, result);
					break;
				}
				case DetourEventType::fopen:
				{
					auto pathname = ReadStringValue(message, offset);
					auto mode = ReadStringValue(message, offset);
					m_callback->OnFOpen(pathname, mode);
					break;
				}
				case DetourEventType::fdopen:
				{
					auto fd = ReadInt32Value(message, offset);
					auto mode = ReadStringValue(message, offset);
					m_callback->OnFDOpen(fd, mode);
					break;
				}
				case DetourEventType::freopen:
				{
					auto pathname = ReadStringValue(message, offset);
					auto mode = ReadStringValue(message, offset);
					m_callback->OnFReopen(pathname, mode);
					break;
				}
				case DetourEventType::mkdir:
				{
					auto path = ReadStringValue(message, offset);
					auto mode = ReadStringValue(message, offset);
					m_callback->OnMkdir(path, mode);
					break;
				}
				// ProcessApi
				case DetourEventType::system:
				{
					auto command = ReadStringValue(message, offset);
					m_callback->OnSystem(command);
					break;
				}
				case DetourEventType::fork:
				{
					m_callback->OnFork();
					break;
				}
				case DetourEventType::execl:
				{
					auto path = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnExecl(path, result);
					break;
				}
				case DetourEventType::execlp:
				{
					auto file = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnExeclp(file, result);
					break;
				}
				case DetourEventType::execle:
				{
					auto path = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnExecle(path, result);
					break;
				}
				case DetourEventType::execv:
				{
					auto path = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnExecv(path, result);
					break;
				}
				case DetourEventType::execvp:
				{
					auto file = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnExecvp(file, result);
					break;
				}
				case DetourEventType::execvpe:
				{
					auto file = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_callback->OnExecvpe(file, result);
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown detour event type");
				}
			}
		}

		bool ReadBoolValue(Message& message, uint32_t& offset)
		{
			auto result = *reinterpret_cast<uint32_t*>(message.Content + offset);
			offset += sizeof(uint32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadBoolValue past end of content");
			return result > 0;
		}

		int32_t ReadInt32Value(Message& message, uint32_t& offset)
		{
			auto result = *reinterpret_cast<int32_t*>(message.Content + offset);
			offset += sizeof(int32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadInt32Value past end of content");
			return result;
		}

		uint32_t ReadUInt32Value(Message& message, uint32_t& offset)
		{
			auto result = *reinterpret_cast<uint32_t*>(message.Content + offset);
			offset += sizeof(uint32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadUInt32Value past end of content");
			return result;
		}

		uint64_t ReadUInt64Value(Message& message, uint32_t& offset)
		{
			auto result = *reinterpret_cast<uint64_t*>(message.Content + offset);
			offset += sizeof(uint64_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadUInt64Value past end of content");
			return result;
		}

		std::string_view ReadStringValue(Message& message, uint32_t& offset)
		{
			auto result = std::string_view(reinterpret_cast<char*>(message.Content + offset));
			offset += static_cast<uint32_t>(result.size()) + 1;
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadStringValue past end of content");
			return result;
		}

		std::wstring_view ReadWStringValue(Message& message, uint32_t& offset)
		{
			auto result = std::wstring_view(reinterpret_cast<wchar_t*>(message.Content + offset));
			offset += 2 * (static_cast<uint32_t>(result.size()) + 1);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadWStringValue past end of content");
			return result;
		}

	private:
		// Input
		std::shared_ptr<IDetourCallback> m_callback;
	};
}
