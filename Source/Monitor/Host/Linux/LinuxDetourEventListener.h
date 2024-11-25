// <copyright file="LinuxDetourEventListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ILinuxSystemMonitor.h"

namespace Monitor::Linux
{
	/// <summary>
	/// The event listener knows how to parse an incoming message and pass it along to the
	/// registered monitor.
	/// </summary>
	class LinuxDetourEventListener
	{
	private:
		// Input
		std::shared_ptr<ILinuxSystemMonitor> m_monitor;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='LinuxDetourEventListener'/> class.
		/// </summary>
		LinuxDetourEventListener(
			std::shared_ptr<ILinuxSystemMonitor> monitor) :
			m_monitor(std::move(monitor))
		{
		}

		void LogError(std::string_view message)
		{
			m_monitor->OnError(message);
		}

		void SafeLogMessage(Message& message)
		{
			try
			{
				LogMessage(message);
			}
			catch (std::exception& ex)
			{
				Log::Error("Event Listener encountered invalid message: {}", ex.what());
			}
		}

	private:
		void LogMessage(Message& message)
		{
			uint32_t offset = 0;
			switch (message.Type)
			{
				// Info
				case MessageType::Initialize:
				{
					m_monitor->OnInitialize();
					break;
				}
				case MessageType::Shutdown:
				{
					auto hadError = ReadBoolValue(message, offset);
					m_monitor->OnShutdown(hadError);
					break;
				}
				case MessageType::Error:
				{
					auto errorMessage = ReadStringValue(message, offset);
					m_monitor->OnError(errorMessage);
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
					m_monitor->OnOpen(path, oflag, result);
					break;
				}
				case DetourEventType::creat:
				{
					auto path = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnCreat(path, result);
					break;
				}
				case DetourEventType::openat:
				{
					auto dirfd = ReadInt32Value(message, offset);
					auto path = ReadStringValue(message, offset);
					auto oflag = ReadInt32Value(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnOpenat(dirfd, path, oflag, result);
					break;
				}
				case DetourEventType::link:
				{
					auto oldpath = ReadStringValue(message, offset);
					auto newpath = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnLink(oldpath, newpath, result);
					break;
				}
				case DetourEventType::linkat:
				{
					auto olddirfd = ReadInt32Value(message, offset);
					auto oldpath = ReadStringValue(message, offset);
					auto newdirfd = ReadInt32Value(message, offset);
					auto newpath = ReadStringValue(message, offset);
					auto flags = ReadInt32Value(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnLinkat(olddirfd, oldpath, newdirfd, newpath, flags, result);
					break;
				}
				case DetourEventType::rename:
				{
					auto oldpath = ReadStringValue(message, offset);
					auto newpath = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnRename(oldpath, newpath, result);
					break;
				}
				case DetourEventType::unlink:
				{
					auto pathname = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnUnlink(pathname, result);
					break;
				}
				case DetourEventType::remove:
				{
					auto pathname = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnRemove(pathname, result);
					break;
				}
				case DetourEventType::fopen:
				{
					auto pathname = ReadStringValue(message, offset);
					auto mode = ReadStringValue(message, offset);
					auto result = ReadUInt64Value(message, offset);
					m_monitor->OnFOpen(pathname, mode, result);
					break;
				}
				case DetourEventType::fdopen:
				{
					auto fd = ReadInt32Value(message, offset);
					auto mode = ReadStringValue(message, offset);
					auto result = ReadUInt64Value(message, offset);
					m_monitor->OnFDOpen(fd, mode, result);
					break;
				}
				case DetourEventType::freopen:
				{
					auto pathname = ReadStringValue(message, offset);
					auto mode = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnFReopen(pathname, mode, result);
					break;
				}
				case DetourEventType::mkdir:
				{
					auto path = ReadStringValue(message, offset);
					auto mode = ReadUInt32Value(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnMkdir(path, mode, result);
					break;
				}
				case DetourEventType::rmdir:
				{
					auto pathname = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnRmdir(pathname, result);
					break;
				}
				// ProcessApi
				case DetourEventType::system:
				{
					auto command = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnSystem(command, result);
					break;
				}
				case DetourEventType::fork:
				{
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnFork(result);
					break;
				}
				case DetourEventType::vfork:
				{
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnVFork(result);
					break;
				}
				case DetourEventType::clone:
				{
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnClone(result);
					break;
				}
				case DetourEventType::__clone2:
				{
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnClone2(result);
					break;
				}
				case DetourEventType::clone3:
				{
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnClone3(result);
					break;
				}
				case DetourEventType::execl:
				{
					auto path = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnExecl(path, result);
					break;
				}
				case DetourEventType::execlp:
				{
					auto file = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnExeclp(file, result);
					break;
				}
				case DetourEventType::execle:
				{
					auto path = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnExecle(path, result);
					break;
				}
				case DetourEventType::execv:
				{
					auto path = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnExecv(path, result);
					break;
				}
				case DetourEventType::execvp:
				{
					auto file = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnExecvp(file, result);
					break;
				}
				case DetourEventType::execvpe:
				{
					auto file = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnExecvpe(file, result);
					break;
				}
				case DetourEventType::execve:
				{
					auto file = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnExecve(file, result);
					break;
				}
				case DetourEventType::execveat:
				{
					auto file = ReadStringValue(message, offset);
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnExecveat(file, result);
					break;
				}
				case DetourEventType::fexecve:
				{
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnFexecve(result);
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown detour event type");
				}
			}
		}

	private:
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
	};
}