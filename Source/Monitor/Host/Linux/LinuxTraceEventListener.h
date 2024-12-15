// <copyright file="EventListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ILinuxSystemMonitor.h"

namespace Monitor::Linux
{
	struct SysCallStatus
	{
		long Command;
		long Return;
		long Error;
		std::array<long, 6> Arguments;
	};

	typedef union {
		long val;
		int32_t intData[sizeof(long) / sizeof(int32_t)];
		char data[sizeof(long)];
	} dissected_long_t;
	
	/// <summary>
	/// The event listener knows how to parse an incoming message and pass it along to the
	/// registered monitor.
	/// </summary>
	class LinuxTraceEventListener
	{
	private:
		// Input
		std::shared_ptr<ILinuxSystemMonitor> m_monitor;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='LinuxTraceEventListener'/> class.
		/// </summary>
		LinuxTraceEventListener(
			std::shared_ptr<ILinuxSystemMonitor> monitor) :
			m_monitor(std::move(monitor))
		{
		}

		void LogError(std::string_view message)
		{
			m_monitor->OnError(message);
		}

		void ProcessSysCall(pid_t pid)
		{
			auto registers = GetSysCallArgs(pid);
			auto args = registers.Arguments;
			auto result = registers.Return;
			switch (registers.Command)
			{
				// FileApi
				case SCMP_SYS(creat):
				{
					auto path = ReadNullTerminatedStringValue(pid, args[0]);
					m_monitor->OnCreat(path, result);
					break;
				}
				case SCMP_SYS(link):
				{
					auto oldpath = ReadNullTerminatedStringValue(pid, args[0]);
					auto newpath = ReadNullTerminatedStringValue(pid, args[1]);
					m_monitor->OnLink(oldpath, newpath, result);
					break;
				}
				case SCMP_SYS(linkat):
				{
					auto olddirfd = (int32_t)args[0];
					auto oldpath = ReadNullTerminatedStringValue(pid, args[1]);
					auto newdirfd = (int32_t)args[2];
					auto newpath = ReadNullTerminatedStringValue(pid, args[3]);
					auto flags = (int32_t)args[4];
					m_monitor->OnLinkAt(olddirfd, oldpath, newdirfd, newpath, flags, result);
					break;
				}
				case SCMP_SYS(mkdir):
				{
					auto path = ReadNullTerminatedStringValue(pid, args[0]);
					auto mode = (uint32_t)args[1];
					m_monitor->OnMkdir(path, mode, result);
					break;
				}
				case SCMP_SYS(mkdirat):
				{
					auto dirfd = (int32_t)args[0];
					auto path = ReadNullTerminatedStringValue(pid, args[1]);
					auto mode = (uint32_t)args[2];
					m_monitor->OnMkdirAt(dirfd, path, mode, result);
					break;
				}
				case SCMP_SYS(open):
				{
					auto path = ReadNullTerminatedStringValue(pid, args[0]);
					auto oflag = (int32_t)args[1];
					m_monitor->OnOpen(path, oflag, result);
					break;
				}
				case SCMP_SYS(openat):
				{
					auto dirfd = (int32_t)args[0];
					auto path = ReadNullTerminatedStringValue(pid, args[1]);
					auto oflag = (int32_t)args[2];
					m_monitor->OnOpenAt(dirfd, path, oflag, result);
					break;
				}
				case SCMP_SYS(openat2):
				{
					auto dirfd = (int32_t)args[0];
					auto path = ReadNullTerminatedStringValue(pid, args[1]);
					auto oflag = (int32_t)args[2];
					m_monitor->OnOpenAt2(dirfd, path, oflag, result);
					break;
				}
				case SCMP_SYS(rename):
				{
					auto oldpath = ReadNullTerminatedStringValue(pid, args[0]);
					auto newpath = ReadNullTerminatedStringValue(pid, args[1]);
					m_monitor->OnRename(oldpath, newpath, result);
					break;
				}
				case SCMP_SYS(renameat):
				{
					auto oldfd = (int32_t)args[0];
					auto oldpath = ReadNullTerminatedStringValue(pid, args[1]);
					auto newfd = (int32_t)args[2];
					auto newpath = ReadNullTerminatedStringValue(pid, args[3]);
					m_monitor->OnRenameAt(oldfd, oldpath, newfd, newpath, result);
					break;
				}
				case SCMP_SYS(renameat2):
				{
					auto oldfd = (int32_t)args[0];
					auto oldpath = ReadNullTerminatedStringValue(pid, args[1]);
					auto newfd = (int32_t)args[2];
					auto newpath = ReadNullTerminatedStringValue(pid, args[3]);
					m_monitor->OnRenameAt2(oldfd, oldpath, newfd, newpath, result);
					break;
				}
				case SCMP_SYS(rmdir):
				{
					auto pathname = ReadNullTerminatedStringValue(pid, args[0]);
					m_monitor->OnRmdir(pathname, result);
					break;
				}
				case SCMP_SYS(unlink):
				{
					auto pathname = ReadNullTerminatedStringValue(pid, args[0]);
					m_monitor->OnUnlink(pathname, result);
					break;
				}

				// ProcessApi
				case SCMP_SYS(clone):
				{
					m_monitor->OnClone(result);
					break;
				}
				case SCMP_SYS(clone3):
				{
					m_monitor->OnClone3(result);
					break;
				}
				case SCMP_SYS(execve):
				{
					auto file = ReadNullTerminatedStringValue(pid, args[0]);
					m_monitor->OnExecve(file, result);
					break;
				}
				case SCMP_SYS(execveat):
				{
					auto file = ReadNullTerminatedStringValue(pid, args[0]);
					m_monitor->OnExecveAt(file, result);
					break;
				}
				case SCMP_SYS(fork):
				{
					m_monitor->OnFork(result);
					break;
				}
				case SCMP_SYS(vfork):
				{
					m_monitor->OnVFork(result);
					break;
				}

				// Uknown
				default:
				{
					throw std::runtime_error("Unknown system call type");
				}
			}
		}

	private:
		SysCallStatus GetSysCallArgs(pid_t pid)
		{
			user_regs_struct regs;
			iovec io;
			io.iov_base = &regs;
			io.iov_len = sizeof(regs);

			if (ptrace(PTRACE_GETREGSET, pid, (void*)NT_PRSTATUS, (void*)&io) == -1)
				throw std::runtime_error("PTRACE_GETREGSET failed");

			auto returnValue = (long)regs.rax;
			auto error = 0;
			if (returnValue < 0)
			{
				error = -returnValue;
				returnValue = -1;
			}

			return {
				(long)regs.orig_rax,
				returnValue,
				error,
				std::array<long, 6>({
					(long)regs.rdi,
					(long)regs.rsi,
					(long)regs.rdx,
					(long)regs.r10,
					(long)regs.r8,
					(long)regs.r9,
				})
			};
		}

		std::string ReadNullTerminatedStringValue(pid_t pid, long addr)
		{
			auto result = std::string(1024, '\0');

			char *laddr = result.data();

			unsigned long len = 1024;
			unsigned int nread = 0;
			unsigned int residue = addr & (sizeof(long) - 1);
			void *const orig_addr = laddr;

			// aligned address
			addr &= -sizeof(long);

			while (len)
			{
				errno = 0;
				dissected_long_t u = {
					.val = ptrace(PTRACE_PEEKDATA, pid, addr, 0)
				};

				switch (errno)
				{
					case 0:
						break;
					case ESRCH:
					case EINVAL:
						throw std::runtime_error("Could be seen if the process is gone");
					case EFAULT:
					case EIO:
					case EPERM:
						throw std::runtime_error("address space is inaccessible");
					default:
						throw std::runtime_error("all the rest is strange and should be reported");
				}

				unsigned long m = std::min(sizeof(long) - residue, len);
				memcpy(laddr, &u.data[residue], m);
				while (residue < sizeof(long))
				{
					if (u.data[residue++] == '\0')
					{
						result.resize(nread + residue - 1);
						return result;
					}
				}

				residue = 0;
				addr += sizeof(long);
				laddr += m;
				nread += m;
				len -= m;
			}

			return result;
		}
	};
}
