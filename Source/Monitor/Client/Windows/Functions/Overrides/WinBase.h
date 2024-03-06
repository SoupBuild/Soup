#pragma once
#include "../Cache/WinBase.h"
#include "FileSystemAccessSandbox.h"

namespace Monitor::Windows::Functions::Overrides::WinBase
{
	BOOL WINAPI CopyFileA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		BOOL bFailIfExists)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CopyFileA(
					lpExistingFileName,
					lpNewFileName,
					bFailIfExists);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(bFailIfExists);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CopyFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		BOOL bFailIfExists)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CopyFileW(
					lpExistingFileName,
					lpNewFileName,
					bFailIfExists);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(bFailIfExists);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HRESULT WINAPI CopyFile2(
		PCWSTR pwszExistingFileName,
		PCWSTR pwszNewFileName,
		COPYFILE2_EXTENDED_PARAMETERS *pExtendedParameters)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(pwszExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(pwszNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		HRESULT result = 0;
		__try
		{
			if (blockAccess)
			{
				result = HRESULT_FROM_WIN32(ERROR_REQUEST_ABORTED);
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CopyFile2(
					pwszExistingFileName,
					pwszNewFileName,
					pExtendedParameters);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFile2));
			message.AppendValue(pwszExistingFileName);
			message.AppendValue(pwszNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CopyFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CopyFileExA(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					pbCancel,
					dwCopyFlags);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileExA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CopyFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CopyFileExW(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					pbCancel,
					dwCopyFlags);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileExW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CopyFileTransactedA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags,
		HANDLE hTransaction)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CopyFileTransactedA(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					pbCancel,
					dwCopyFlags,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileTransactedA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CopyFileTransactedW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags,
		HANDLE hTransaction)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CopyFileTransactedW(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					pbCancel,
					dwCopyFlags,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileTransactedW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryExA(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CreateDirectoryExA(
					lpTemplateDirectory,
					lpNewDirectory,
					lpSecurityAttributes);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryExA));
			message.AppendValue(lpTemplateDirectory);
			message.AppendValue(lpNewDirectory);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryExW(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CreateDirectoryExW(
					lpTemplateDirectory,
					lpNewDirectory,
					lpSecurityAttributes);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryExW));
			message.AppendValue(lpTemplateDirectory);
			message.AppendValue(lpNewDirectory);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryTransactedA(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CreateDirectoryTransactedA(
					lpTemplateDirectory,
					lpNewDirectory,
					lpSecurityAttributes,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryTransactedA));
			message.AppendValue(lpTemplateDirectory);
			message.AppendValue(lpNewDirectory);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryTransactedW(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CreateDirectoryTransactedW(
					lpTemplateDirectory,
					lpNewDirectory,
					lpSecurityAttributes,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryTransactedW));
			message.AppendValue(lpTemplateDirectory);
			message.AppendValue(lpNewDirectory);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI CreateFileTransactedA(
		LPCSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile,
		HANDLE hTransaction,
		PUSHORT pusMiniVersion,
		PVOID lpExtendedParameter)
	{
		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CreateFileTransactedA(
					lpFileName,
					dwDesiredAccess,
					dwShareMode,
					lpSecurityAttributes,
					dwCreationDisposition,
					dwFlagsAndAttributes,
					hTemplateFile,
					hTransaction,
					pusMiniVersion,
					lpExtendedParameter);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFileTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI CreateFileTransactedW(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile,
		HANDLE hTransaction,
		PUSHORT pusMiniVersion,
		PVOID lpExtendedParameter)
	{
		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::CreateFileTransactedW(
					lpFileName,
					dwDesiredAccess,
					dwShareMode,
					lpSecurityAttributes,
					dwCreationDisposition,
					dwFlagsAndAttributes,
					hTemplateFile,
					hTransaction,
					pusMiniVersion,
					lpExtendedParameter);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFileTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateHardLinkA(
		LPCSTR lpFileName,
		LPCSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::CreateHardLinkA(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateHardLinkA));
			message.AppendValue(lpFileName);
			message.AppendValue(lpExistingFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateHardLinkW(
		LPCWSTR lpFileName,
		LPCWSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::CreateHardLinkW(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes);;
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateHardLinkW));
			message.AppendValue(lpFileName);
			message.AppendValue(lpExistingFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateHardLinkTransactedA(
		LPCSTR lpFileName,
		LPCSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::CreateHardLinkTransactedA(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateHardLinkTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(lpExistingFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateHardLinkTransactedW(
		LPCWSTR lpFileName,
		LPCWSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::CreateHardLinkTransactedW(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateHardLinkTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(lpExistingFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateProcessWithLogonW(
		LPCWSTR lpUsername,
		LPCWSTR lpDomain,
		LPCWSTR lpPassword,
		DWORD dwLogonFlags,
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::CreateProcessWithLogonW(
				lpUsername,
				lpDomain,
				lpPassword,
				dwLogonFlags,
				lpApplicationName,
				lpCommandLine,
				dwCreationFlags,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				lpProcessInformation);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateProcessWithLogonW));
			message.AppendValue(lpApplicationName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);

			// TODO
			{
				auto errorMessage = MessageSender(MessageType::Error);
				errorMessage.AppendValue("Cannot detour process from CreateProcessWithLogonW");
			}
		}

		return result;
	}

	BOOL WINAPI CreateProcessWithTokenW(
		HANDLE hToken,
		DWORD dwLogonFlags,
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::CreateProcessWithTokenW(
				hToken,
				dwLogonFlags,
				lpApplicationName,
				lpCommandLine,
				dwCreationFlags,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				lpProcessInformation);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateProcessWithTokenW));
			message.AppendValue(lpApplicationName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);

			// TODO
			{
				auto errorMessage = MessageSender(MessageType::Error);
				errorMessage.AppendValue("Cannot detour process from CreateProcessWithTokenW");
			}
		}

		return result;
	}

	BOOLEAN WINAPI CreateSymbolicLinkA(
		LPCSTR lpSymlinkFileName,
		LPCSTR lpTargetFileName,
		DWORD dwFlags)
	{
		BOOLEAN result = 0;
		__try
		{
			result = Cache::WinBase::CreateSymbolicLinkA(
				lpSymlinkFileName,
				lpTargetFileName,
				dwFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateSymbolicLinkA));
			message.AppendValue(lpSymlinkFileName);
			message.AppendValue(lpTargetFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOLEAN WINAPI CreateSymbolicLinkW(
		LPCWSTR lpSymlinkFileName,
		LPCWSTR lpTargetFileName,
		DWORD dwFlags)
	{
		BOOLEAN result = 0;
		__try
		{
			result = Cache::WinBase::CreateSymbolicLinkW(
				lpSymlinkFileName,
				lpTargetFileName,
				dwFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateSymbolicLinkW));
			message.AppendValue(lpSymlinkFileName);
			message.AppendValue(lpTargetFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOLEAN WINAPI CreateSymbolicLinkTransactedA(
		LPCSTR lpSymlinkFileName,
		LPCSTR lpTargetFileName,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		BOOLEAN result = 0;
		__try
		{
			result = Cache::WinBase::CreateSymbolicLinkTransactedA(
				lpSymlinkFileName,
				lpTargetFileName,
				dwFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateSymbolicLinkTransactedA));
			message.AppendValue(lpSymlinkFileName);
			message.AppendValue(lpTargetFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOLEAN WINAPI CreateSymbolicLinkTransactedW(
		LPCWSTR lpSymlinkFileName,
		LPCWSTR lpTargetFileName,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		BOOLEAN result = 0;
		__try
		{
			result = Cache::WinBase::CreateSymbolicLinkTransactedW(
				lpSymlinkFileName,
				lpTargetFileName,
				dwFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateSymbolicLinkTransactedW));
			message.AppendValue(lpSymlinkFileName);
			message.AppendValue(lpTargetFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DecryptFileA(
		LPCSTR lpFileName,
		DWORD dwReserved)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::DecryptFileA(
				lpFileName,
				dwReserved);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DecryptFileA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DecryptFileW(
		LPCWSTR lpFileName,
		DWORD dwReserved)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::DecryptFileW(
				lpFileName,
				dwReserved);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DecryptFileW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFile(
		LPCSTR lpFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::DeleteFile(
				lpFileName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFile));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileTransactedA(
		LPCSTR lpFileName,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::DeleteFileTransactedA(
					lpFileName,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFileTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileTransactedW(
		LPCWSTR lpFileName,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::DeleteFileTransactedW(
					lpFileName,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFileTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI EncryptFileA(
		LPCSTR lpFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::EncryptFileA(
				lpFileName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::EncryptFileA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI EncryptFileW(
		LPCWSTR lpFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::EncryptFileW(
				lpFileName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::EncryptFileW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FileEncryptionStatusA(
		LPCSTR lpFileName,
		LPDWORD lpStatus)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::FileEncryptionStatusA(
				lpFileName,
				lpStatus);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FileEncryptionStatusA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FileEncryptionStatusW(
		LPCWSTR lpFileName,
		LPDWORD lpStatus)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::FileEncryptionStatusW(
				lpFileName,
				lpStatus);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FileEncryptionStatusW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstFileNameTransactedW(
		LPCWSTR lpFileName,
		DWORD dwFlags,
		LPDWORD StringLength,
		PWSTR LinkName,
		HANDLE hTransaction)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::WinBase::FindFirstFileNameTransactedW(
				lpFileName,
				dwFlags,
				StringLength,
				LinkName,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileNameTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwFlags);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstFileTransactedA(
		LPCSTR lpFileName,
		FINDEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFindFileData,
		FINDEX_SEARCH_OPS fSearchOp,
		LPVOID lpSearchFilter,
		DWORD dwAdditionalFlags,
		HANDLE hTransaction)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::WinBase::FindFirstFileTransactedA(
				lpFileName,
				fInfoLevelId,
				lpFindFileData,
				fSearchOp,
				lpSearchFilter,
				dwAdditionalFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileTransactedA));
			message.AppendValue(lpFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstFileTransactedW(
		LPCWSTR lpFileName,
		FINDEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFindFileData,
		FINDEX_SEARCH_OPS fSearchOp,
		LPVOID lpSearchFilter,
		DWORD dwAdditionalFlags,
		HANDLE hTransaction)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::WinBase::FindFirstFileTransactedW(
				lpFileName,
				fInfoLevelId,
				lpFindFileData,
				fSearchOp,
				lpSearchFilter,
				dwAdditionalFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileTransactedW));
			message.AppendValue(lpFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstStreamTransactedW(
		LPCWSTR lpFileName,
		STREAM_INFO_LEVELS InfoLevel,
		LPVOID lpFindStreamData,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::WinBase::FindFirstStreamTransactedW(
				lpFileName,
				InfoLevel,
				lpFindStreamData,
				dwFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstStreamTransactedW));
			message.AppendValue(lpFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetBinaryTypeA(
		LPCSTR lpApplicationName,
		LPDWORD lpBinaryType)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::GetBinaryTypeA(
				lpApplicationName,
				lpBinaryType);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetBinaryTypeA));
			message.AppendValue(lpApplicationName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetBinaryTypeW(
		LPCWSTR lpApplicationName,
		LPDWORD lpBinaryType)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::GetBinaryTypeW(
				lpApplicationName,
				lpBinaryType);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetBinaryTypeW));
			message.AppendValue(lpApplicationName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetCompressedFileSizeTransactedA(
		LPCSTR lpFileName,
		LPDWORD lpFileSizeHigh,
		HANDLE hTransaction)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetCompressedFileSizeTransactedA(
				lpFileName,
				lpFileSizeHigh,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetCompressedFileSizeTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetCompressedFileSizeTransactedW(
		LPCWSTR lpFileName,
		LPDWORD lpFileSizeHigh,
		HANDLE hTransaction)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetCompressedFileSizeTransactedW(
				lpFileName,
				lpFileSizeHigh,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetCompressedFileSizeTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetDllDirectoryA(
		DWORD nBufferLength,
		LPSTR lpBuffer)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetDllDirectoryA(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDllDirectoryA));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetDllDirectoryW(
		DWORD nBufferLength,
		LPWSTR lpBuffer)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::GetDllDirectoryW(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDllDirectoryW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetEnvironmentVariableA(
		LPCTSTR lpName,
		LPTSTR lpBuffer,
		DWORD nSize)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetEnvironmentVariableA(
				lpName,
				lpBuffer,
				nSize);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetEnvironmentVariableA));
			message.AppendValue(lpName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetEnvironmentVariableW(
		LPCWSTR lpName,
		LPWSTR lpBuffer,
		DWORD nSize)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetEnvironmentVariableW(
				lpName,
				lpBuffer,
				nSize);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetEnvironmentVariableW));
			message.AppendValue(lpName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesTransactedA(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation,
		HANDLE hTransaction)
	{
		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::GetFileAttributesTransactedA(
					lpFileName,
					fInfoLevelId,
					lpFileInformation,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesTransactedW(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation,
		HANDLE hTransaction)
	{
		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::GetFileAttributesTransactedW(
					lpFileName,
					fInfoLevelId,
					lpFileInformation,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileBandwidthReservation(
		HANDLE hFile,
		LPDWORD lpPeriodMilliseconds,
		LPDWORD lpBytesPerPeriod,
		LPBOOL pDiscardable,
		LPDWORD lpTransferSize,
		LPDWORD lpNumOutstandingRequests)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::GetFileBandwidthReservation(
				hFile,
				lpPeriodMilliseconds,
				lpBytesPerPeriod,
				pDiscardable,
				lpTransferSize,
				lpNumOutstandingRequests);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileBandwidthReservation));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileInformationByHandleEx(
		HANDLE hFile,
		FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
		LPVOID lpFileInformation,
		DWORD dwBufferSize)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::GetFileInformationByHandleEx(
				hFile,
				FileInformationClass,
				lpFileInformation,
				dwBufferSize);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileInformationByHandleEx));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileSecurityA(
		LPCSTR lpFileName,
		SECURITY_INFORMATION RequestedInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor,
		DWORD nLength,
		LPDWORD lpnLengthNeeded)
	{
		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::GetFileSecurityA(
					lpFileName,
					RequestedInformation,
					pSecurityDescriptor,
					nLength,
					lpnLengthNeeded);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileSecurityA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFullPathNameTransactedA(
		LPCSTR lpFileName,
		DWORD nBufferLength,
		LPSTR lpBuffer,
		LPSTR *lpFilePart,
		HANDLE hTransaction)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetFullPathNameTransactedA(
				lpFileName,
				nBufferLength,
				lpBuffer,
				lpFilePart,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFullPathNameTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFullPathNameTransactedW(
		LPCWSTR lpFileName,
		DWORD nBufferLength,
		LPWSTR lpBuffer,
		LPWSTR *lpFilePart,
		HANDLE hTransaction)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetFullPathNameTransactedW(
				lpFileName,
				nBufferLength,
				lpBuffer,
				lpFilePart,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFullPathNameTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetLongPathNameTransactedA(
		LPCSTR lpszShortPath,
		LPSTR lpszLongPath,
		DWORD cchBuffer,
		HANDLE hTransaction)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetLongPathNameTransactedA(
				lpszShortPath,
				lpszLongPath,
				cchBuffer,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLongPathNameTransactedA));
			message.AppendValue(lpszShortPath);
			message.AppendValue(lpszLongPath);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetLongPathNameTransactedW(
		LPCWSTR lpszShortPath,
		LPWSTR lpszLongPath,
		DWORD cchBuffer,
		HANDLE hTransaction)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::GetLongPathNameTransactedW(
				lpszShortPath,
				lpszLongPath,
				cchBuffer,
				hTransaction);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLongPathNameTransactedW));
			message.AppendValue(lpszShortPath);
			message.AppendValue(lpszLongPath);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetQueuedCompletionStatus(
		HANDLE CompletionPort,
		LPDWORD lpNumberOfBytesTransferred,
		PULONG_PTR lpCompletionKey,
		LPOVERLAPPED *lpOverlapped,
		DWORD dwMilliseconds)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::GetQueuedCompletionStatus(
				CompletionPort,
				lpNumberOfBytesTransferred,
				lpCompletionKey,
				lpOverlapped,
				dwMilliseconds);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetQueuedCompletionStatus));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetQueuedCompletionStatusEx(
		HANDLE CompletionPort,
		LPOVERLAPPED_ENTRY lpCompletionPortEntries,
		ULONG ulCount,
		PULONG ulNumEntriesRemoved,
		DWORD dwMilliseconds,
		BOOL fAlertable)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::GetQueuedCompletionStatusEx(
				CompletionPort,
				lpCompletionPortEntries,
				ulCount,
				ulNumEntriesRemoved,
				dwMilliseconds,
				fAlertable);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetQueuedCompletionStatusEx));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetShortPathNameA(
		LPCSTR lpszLongPath,
		LPSTR lpszShortPath,
		DWORD cchBuffer)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::GetShortPathNameA(
				lpszLongPath,
				lpszShortPath,
				cchBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetShortPathNameA));
			message.AppendValue(lpszLongPath);
			message.AppendValue(lpszShortPath);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI LoadModule(
		LPCSTR lpModuleName,
		LPVOID lpParameterBlock)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::LoadModule(
				lpModuleName,
				lpParameterBlock);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadModule));
			message.AppendValue(lpModuleName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HMODULE WINAPI LoadPackagedLibrary(
		LPCWSTR lpwLibFileName,
		DWORD Reserved)
	{
		HMODULE result = 0;
		__try
		{
			result = Cache::WinBase::LoadPackagedLibrary(
				lpwLibFileName,
				Reserved);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadPackagedLibrary));
			message.AppendValue(lpwLibFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileA(
		LPCTSTR lpExistingFileName,
		LPCTSTR lpNewFileName)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::MoveFileA(
					lpExistingFileName,
					lpNewFileName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::MoveFileW(
					lpExistingFileName,
					lpNewFileName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		DWORD dwFlags)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::MoveFileExA(
					lpExistingFileName,
					lpNewFileName,
					dwFlags);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileExA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		DWORD dwFlags)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::MoveFileExW(
					lpExistingFileName,
					lpNewFileName,
					dwFlags);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileExW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileTransactedA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::MoveFileTransactedA(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					dwFlags,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileTransactedA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileTransactedW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::MoveFileTransactedW(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					dwFlags,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileTransactedW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileWithProgressA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::MoveFileWithProgressA(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					dwFlags);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileWithProgressA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileWithProgressW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::MoveFileWithProgressW(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					dwFlags);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileWithProgressW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI OpenEncryptedFileRawA(
		LPCSTR lpFileName,
		ULONG ulFlags,
		PVOID *pvContext)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::OpenEncryptedFileRawA(
				lpFileName,
				ulFlags,
				pvContext);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::OpenEncryptedFileRawA));
			message.AppendValue(lpFileName);
			message.AppendValue(ulFlags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI OpenEncryptedFileRawW(
		LPCWSTR lpFileName,
		ULONG ulFlags,
		PVOID *pvContext)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::OpenEncryptedFileRawW(
				lpFileName,
				ulFlags,
				pvContext);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::OpenEncryptedFileRawW));
			message.AppendValue(lpFileName);
			message.AppendValue(ulFlags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HFILE WINAPI OpenFile(
		LPCSTR lpFileName,
		LPOFSTRUCT lpReOpenBuff,
		UINT uStyle)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		HFILE result = 0;
		__try
		{
			if (blockAccess)
			{
				result = 0;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::OpenFile(
					lpFileName,
					lpReOpenBuff,
					uStyle);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::OpenFile));
			message.AppendValue(lpFileName);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI OpenFileById(
		HANDLE hVolumeHint,
		LPFILE_ID_DESCRIPTOR lpFileId,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwFlagsAndAttributes)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::WinBase::OpenFileById(
				hVolumeHint,
				lpFileId,
				dwDesiredAccess,
				dwShareMode,
				lpSecurityAttributes,
				dwFlagsAndAttributes);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::OpenFileById));
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI ReadEncryptedFileRaw(
		PFE_EXPORT_FUNC pfExportCallback,
		PVOID pvCallbackContext,
		PVOID pvContext)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::WinBase::ReadEncryptedFileRaw(
				pfExportCallback,
				pvCallbackContext,
				pvContext);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::ReadEncryptedFileRaw));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryTransactedA(
		LPCSTR lpPathName,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpPathName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::RemoveDirectoryTransactedA(
					lpPathName,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::RemoveDirectoryTransactedA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryTransactedW(
		LPCWSTR lpPathName,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpPathName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::RemoveDirectoryTransactedW(
					lpPathName,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::RemoveDirectoryTransactedW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI ReOpenFile(
		HANDLE hOriginalFile,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwFlagsAndAttributes)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::WinBase::ReOpenFile(
				hOriginalFile,
				dwDesiredAccess,
				dwShareMode,
				dwFlagsAndAttributes);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::ReOpenFile));
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI ReplaceFileA(
		LPCSTR lpReplacedFileName,
		LPCSTR lpReplacementFileName,
		LPCSTR lpBackupFileName,
		DWORD dwReplaceFlags,
		LPVOID lpExclude,
		LPVOID lpReserved)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::ReplaceFileA(
				lpReplacedFileName,
				lpReplacementFileName,
				lpBackupFileName,
				dwReplaceFlags,
				lpExclude,
				lpReserved);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::ReplaceFileA));
			message.AppendValue(lpReplacedFileName);
			message.AppendValue(lpReplacementFileName);
			message.AppendValue(lpBackupFileName);
			message.AppendValue(dwReplaceFlags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI ReplaceFileW(
		LPCWSTR lpReplacedFileName,
		LPCWSTR lpReplacementFileName,
		LPCWSTR lpBackupFileName,
		DWORD dwReplaceFlags,
		LPVOID lpExclude,
		LPVOID lpReserved)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::ReplaceFileW(
				lpReplacedFileName,
				lpReplacementFileName,
				lpBackupFileName,
				dwReplaceFlags,
				lpExclude,
				lpReserved);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::ReplaceFileW));
			message.AppendValue(lpReplacedFileName);
			message.AppendValue(lpReplacementFileName);
			message.AppendValue(lpBackupFileName);
			message.AppendValue(dwReplaceFlags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetCurrentDirectoryA(
		LPCTSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetCurrentDirectoryA(
				lpPathName);

			if (result)
			{
				FileSystemAccessSandbox::UpdateWorkingDirectory(lpPathName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetCurrentDirectoryA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetCurrentDirectoryW(
		LPCWSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetCurrentDirectoryW(
				lpPathName);

			if (result)
			{
				FileSystemAccessSandbox::UpdateWorkingDirectory(lpPathName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetCurrentDirectoryW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetDllDirectoryA(
		LPCSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetDllDirectoryA(
				lpPathName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetDllDirectoryA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetDllDirectoryW(
		LPCWSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetDllDirectoryW(
				lpPathName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetDllDirectoryW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesTransactedA(
		LPCSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::SetFileAttributesTransactedA(
					lpFileName,
					dwFileAttributes,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileAttributesTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(dwFileAttributes);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesTransactedW(
		LPCWSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::WinBase::SetFileAttributesTransactedW(
					lpFileName,
					dwFileAttributes,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileAttributesTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwFileAttributes);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileBandwidthReservation(
		HANDLE hFile,
		DWORD nPeriodMilliseconds,
		DWORD nBytesPerPeriod,
		BOOL bDiscardable,
		LPDWORD lpTransferSize,
		LPDWORD lpNumOutstandingRequests)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetFileBandwidthReservation(
				hFile,
				nPeriodMilliseconds,
				nBytesPerPeriod,
				bDiscardable,
				lpTransferSize,
				lpNumOutstandingRequests);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileBandwidthReservation));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileCompletionNotificationModes(
		HANDLE FileHandle,
		UCHAR Flags)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetFileCompletionNotificationModes(
				FileHandle,
				Flags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileCompletionNotificationModes));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileSecurityA(
		LPCSTR lpFileName,
		SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetFileSecurityA(
				lpFileName,
				SecurityInformation,
				pSecurityDescriptor);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileSecurityA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileShortNameA(
		HANDLE hFile,
		LPCSTR lpShortName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetFileShortNameA(
				hFile,
				lpShortName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileShortNameA));
			message.AppendValue(lpShortName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileShortNameW(
		HANDLE hFile,
		LPCWSTR lpShortName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetFileShortNameW(
				hFile,
				lpShortName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileShortNameW));
			message.AppendValue(lpShortName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetSearchPathMode(
		DWORD Flags)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::SetSearchPathMode(
				Flags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetSearchPathMode));
			message.AppendValue(Flags);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI WriteEncryptedFileRaw(
		PFE_IMPORT_FUNC pfImportCallback,
		PVOID pvCallbackContext,
		PVOID pvContext)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::WinBase::WriteEncryptedFileRaw(
				pfImportCallback,
				pvCallbackContext,
				pvContext);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::WriteEncryptedFileRaw));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}
}