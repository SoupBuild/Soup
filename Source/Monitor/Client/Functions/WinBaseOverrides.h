#pragma once
#include "WinBaseCache.h"

namespace Functions::WinBase::Overrides
{
	BOOL WINAPI CopyFileA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		BOOL bFailIfExists)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::CopyFileA(
					lpExistingFileName,
					lpNewFileName,
					bFailIfExists);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CopyFileA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(bFailIfExists);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CopyFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		BOOL bFailIfExists)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::CopyFileW(
					lpExistingFileName,
					lpNewFileName,
					bFailIfExists);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CopyFileW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(bFailIfExists);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	HRESULT WINAPI CopyFile2(
		PCWSTR pwszExistingFileName,
		PCWSTR pwszNewFileName,
		COPYFILE2_EXTENDED_PARAMETERS *pExtendedParameters)
	{
		// Check if this file is allowed access
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(pwszExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(pwszNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		HRESULT result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::CopyFile2(
					pwszExistingFileName,
					pwszNewFileName,
					pExtendedParameters);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CopyFile2));
			message.AppendValue(pwszExistingFileName);
			message.AppendValue(pwszNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::CopyFileExA(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CopyFileExA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::CopyFileExW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CopyFileExW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::CopyFileTransactedA(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CopyFileTransactedA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::CopyFileTransactedW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CopyFileTransactedW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryExA(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
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
				result = Cache::CreateDirectoryExA(
					lpTemplateDirectory,
					lpNewDirectory,
					lpSecurityAttributes);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateDirectoryExA));
			message.AppendValue(lpTemplateDirectory);
			message.AppendValue(lpNewDirectory);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryExW(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
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
				result = Cache::CreateDirectoryExW(
					lpTemplateDirectory,
					lpNewDirectory,
					lpSecurityAttributes);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateDirectoryExW));
			message.AppendValue(lpTemplateDirectory);
			message.AppendValue(lpNewDirectory);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
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
				result = Cache::CreateDirectoryTransactedA(
					lpTemplateDirectory,
					lpNewDirectory,
					lpSecurityAttributes,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateDirectoryTransactedA));
			message.AppendValue(lpTemplateDirectory);
			message.AppendValue(lpNewDirectory);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
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
				result = Cache::CreateDirectoryTransactedW(
					lpTemplateDirectory,
					lpNewDirectory,
					lpSecurityAttributes,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateDirectoryTransactedW));
			message.AppendValue(lpTemplateDirectory);
			message.AppendValue(lpNewDirectory);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
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
				result = Cache::CreateFileTransactedA(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateFileTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
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
				result = Cache::CreateFileTransactedW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateFileTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CreateHardLinkA(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateHardLinkA));
			message.AppendValue(lpFileName);
			message.AppendValue(lpExistingFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CreateHardLinkW(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes);;
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateHardLinkW));
			message.AppendValue(lpFileName);
			message.AppendValue(lpExistingFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CreateHardLinkTransactedA(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateHardLinkTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(lpExistingFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CreateHardLinkTransactedW(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateHardLinkTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(lpExistingFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CreateProcessWithLogonW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateProcessWithLogonW));
			message.AppendValue(lpApplicationName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);

			// TODO
			Monitor::ConnectionManager::WriteError("Cannot detour process from CreateProcessWithLogonW");
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
			result = Cache::CreateProcessWithTokenW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateProcessWithTokenW));
			message.AppendValue(lpApplicationName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);

			// TODO
			Monitor::ConnectionManager::WriteError("Cannot detour process from CreateProcessWithTokenW");
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
			result = Cache::CreateSymbolicLinkA(
				lpSymlinkFileName,
				lpTargetFileName,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateSymbolicLinkA));
			message.AppendValue(lpSymlinkFileName);
			message.AppendValue(lpTargetFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CreateSymbolicLinkW(
				lpSymlinkFileName,
				lpTargetFileName,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateSymbolicLinkW));
			message.AppendValue(lpSymlinkFileName);
			message.AppendValue(lpTargetFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CreateSymbolicLinkTransactedA(
				lpSymlinkFileName,
				lpTargetFileName,
				dwFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateSymbolicLinkTransactedA));
			message.AppendValue(lpSymlinkFileName);
			message.AppendValue(lpTargetFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CreateSymbolicLinkTransactedW(
				lpSymlinkFileName,
				lpTargetFileName,
				dwFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateSymbolicLinkTransactedW));
			message.AppendValue(lpSymlinkFileName);
			message.AppendValue(lpTargetFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::DecryptFileA(
				lpFileName,
				dwReserved);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DecryptFileA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::DecryptFileW(
				lpFileName,
				dwReserved);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DecryptFileW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFile(
		LPCSTR lpFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::DeleteFile(
				lpFileName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DeleteFile));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileTransactedA(
		LPCSTR lpFileName,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
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
				result = Cache::DeleteFileTransactedA(
					lpFileName,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DeleteFileTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileTransactedW(
		LPCWSTR lpFileName,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
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
				result = Cache::DeleteFileTransactedW(
					lpFileName,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DeleteFileTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI EncryptFileA(
		LPCSTR lpFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::EncryptFileA(
				lpFileName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::EncryptFileA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI EncryptFileW(
		LPCWSTR lpFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::EncryptFileW(
				lpFileName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::EncryptFileW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FileEncryptionStatusA(
				lpFileName,
				lpStatus);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FileEncryptionStatusA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FileEncryptionStatusW(
				lpFileName,
				lpStatus);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FileEncryptionStatusW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstFileNameTransactedW(
				lpFileName,
				dwFlags,
				StringLength,
				LinkName,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstFileNameTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwFlags);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstFileTransactedA(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstFileTransactedA));
			message.AppendValue(lpFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstFileTransactedW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstFileTransactedW));
			message.AppendValue(lpFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstStreamTransactedW(
				lpFileName,
				InfoLevel,
				lpFindStreamData,
				dwFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstStreamTransactedW));
			message.AppendValue(lpFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetBinaryTypeA(
				lpApplicationName,
				lpBinaryType);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetBinaryTypeA));
			message.AppendValue(lpApplicationName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetBinaryTypeW(
				lpApplicationName,
				lpBinaryType);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetBinaryTypeW));
			message.AppendValue(lpApplicationName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetCompressedFileSizeTransactedA(
				lpFileName,
				lpFileSizeHigh,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetCompressedFileSizeTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetCompressedFileSizeTransactedW(
				lpFileName,
				lpFileSizeHigh,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetCompressedFileSizeTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetDllDirectoryA(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetDllDirectoryA));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetDllDirectoryW(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetDllDirectoryW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpFileName);
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
				result = Cache::GetFileAttributesTransactedA(
					lpFileName,
					fInfoLevelId,
					lpFileInformation,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileAttributesTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpFileName);
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
				result = Cache::GetFileAttributesTransactedW(
					lpFileName,
					fInfoLevelId,
					lpFileInformation,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileAttributesTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFileBandwidthReservation(
				hFile,
				lpPeriodMilliseconds,
				lpBytesPerPeriod,
				pDiscardable,
				lpTransferSize,
				lpNumOutstandingRequests);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileBandwidthReservation));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFileInformationByHandleEx(
				hFile,
				FileInformationClass,
				lpFileInformation,
				dwBufferSize);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileInformationByHandleEx));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpFileName);
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
				result = Cache::GetFileSecurityA(
					lpFileName,
					RequestedInformation,
					pSecurityDescriptor,
					nLength,
					lpnLengthNeeded);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileSecurityA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFullPathNameTransactedA(
				lpFileName,
				nBufferLength,
				lpBuffer,
				lpFilePart,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFullPathNameTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFullPathNameTransactedW(
				lpFileName,
				nBufferLength,
				lpBuffer,
				lpFilePart,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFullPathNameTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetLongPathNameTransactedA(
				lpszShortPath,
				lpszLongPath,
				cchBuffer,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetLongPathNameTransactedA));
			message.AppendValue(lpszShortPath);
			message.AppendValue(lpszLongPath);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetLongPathNameTransactedW(
				lpszShortPath,
				lpszLongPath,
				cchBuffer,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetLongPathNameTransactedW));
			message.AppendValue(lpszShortPath);
			message.AppendValue(lpszLongPath);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetQueuedCompletionStatus(
				CompletionPort,
				lpNumberOfBytesTransferred,
				lpCompletionKey,
				lpOverlapped,
				dwMilliseconds);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetQueuedCompletionStatus));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetQueuedCompletionStatusEx(
				CompletionPort,
				lpCompletionPortEntries,
				ulCount,
				ulNumEntriesRemoved,
				dwMilliseconds,
				fAlertable);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetQueuedCompletionStatusEx));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetShortPathNameA(
				lpszLongPath,
				lpszShortPath,
				cchBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetShortPathNameA));
			message.AppendValue(lpszLongPath);
			message.AppendValue(lpszShortPath);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::LoadModule(
				lpModuleName,
				lpParameterBlock);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LoadModule));
			message.AppendValue(lpModuleName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::LoadPackagedLibrary(
				lpwLibFileName,
				Reserved);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LoadPackagedLibrary));
			message.AppendValue(lpwLibFileName);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileA(
		LPCTSTR lpExistingFileName,
		LPCTSTR lpNewFileName)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::MoveFileA(
					lpExistingFileName,
					lpNewFileName);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::MoveFileA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::MoveFileW(
					lpExistingFileName,
					lpNewFileName);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::MoveFileW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		DWORD dwFlags)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::MoveFileExA(
					lpExistingFileName,
					lpNewFileName,
					dwFlags);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::MoveFileExA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		DWORD dwFlags)
	{
		// Check if this file is allowed write access
		bool blockExistingFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::MoveFileExW(
					lpExistingFileName,
					lpNewFileName,
					dwFlags);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::MoveFileExW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockExistingFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::MoveFileTransactedA(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::MoveFileTransactedA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockExistingFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::MoveFileTransactedW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::MoveFileTransactedW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockExistingFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::MoveFileWithProgressA(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					dwFlags);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::MoveFileWithProgressA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockExistingFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
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
				result = Cache::MoveFileWithProgressW(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					dwFlags);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::MoveFileWithProgressW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(dwFlags);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::OpenEncryptedFileRawA(
				lpFileName,
				ulFlags,
				pvContext);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::OpenEncryptedFileRawA));
			message.AppendValue(lpFileName);
			message.AppendValue(ulFlags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::OpenEncryptedFileRawW(
				lpFileName,
				ulFlags,
				pvContext);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::OpenEncryptedFileRawW));
			message.AppendValue(lpFileName);
			message.AppendValue(ulFlags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	HFILE WINAPI OpenFile(
		LPCSTR lpFileName,
		LPOFSTRUCT lpReOpenBuff,
		UINT uStyle)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
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
				result = Cache::OpenFile(
					lpFileName,
					lpReOpenBuff,
					uStyle);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::OpenFile));
			message.AppendValue(lpFileName);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::OpenFileById(
				hVolumeHint,
				lpFileId,
				dwDesiredAccess,
				dwShareMode,
				lpSecurityAttributes,
				dwFlagsAndAttributes);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::OpenFileById));
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::ReadEncryptedFileRaw(
				pfExportCallback,
				pvCallbackContext,
				pvContext);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::ReadEncryptedFileRaw));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryTransactedA(
		LPCSTR lpPathName,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpPathName);
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
				result = Cache::RemoveDirectoryTransactedA(
					lpPathName,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::RemoveDirectoryTransactedA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryTransactedW(
		LPCWSTR lpPathName,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpPathName);
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
				result = Cache::RemoveDirectoryTransactedW(
					lpPathName,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::RemoveDirectoryTransactedW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::ReOpenFile(
				hOriginalFile,
				dwDesiredAccess,
				dwShareMode,
				dwFlagsAndAttributes);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::ReOpenFile));
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::ReplaceFileA(
				lpReplacedFileName,
				lpReplacementFileName,
				lpBackupFileName,
				dwReplaceFlags,
				lpExclude,
				lpReserved);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::ReplaceFileA));
			message.AppendValue(lpReplacedFileName);
			message.AppendValue(lpReplacementFileName);
			message.AppendValue(lpBackupFileName);
			message.AppendValue(dwReplaceFlags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::ReplaceFileW(
				lpReplacedFileName,
				lpReplacementFileName,
				lpBackupFileName,
				dwReplaceFlags,
				lpExclude,
				lpReserved);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::ReplaceFileW));
			message.AppendValue(lpReplacedFileName);
			message.AppendValue(lpReplacementFileName);
			message.AppendValue(lpBackupFileName);
			message.AppendValue(dwReplaceFlags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetCurrentDirectoryA(
		LPCTSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetCurrentDirectoryA(
				lpPathName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetCurrentDirectoryA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetCurrentDirectoryW(
		LPCWSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetCurrentDirectoryW(
				lpPathName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetCurrentDirectoryW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetDllDirectoryA(
		LPCSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetDllDirectoryA(
				lpPathName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetDllDirectoryA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetDllDirectoryW(
		LPCWSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetDllDirectoryW(
				lpPathName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetDllDirectoryW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesTransactedA(
		LPCSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
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
				result = Cache::SetFileAttributesTransactedA(
					lpFileName,
					dwFileAttributes,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileAttributesTransactedA));
			message.AppendValue(lpFileName);
			message.AppendValue(dwFileAttributes);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesTransactedW(
		LPCWSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction)
	{
		// Check if this file is allowed write access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
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
				result = Cache::SetFileAttributesTransactedW(
					lpFileName,
					dwFileAttributes,
					hTransaction);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileAttributesTransactedW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwFileAttributes);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileBandwidthReservation(
				hFile,
				nPeriodMilliseconds,
				nBytesPerPeriod,
				bDiscardable,
				lpTransferSize,
				lpNumOutstandingRequests);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileBandwidthReservation));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileCompletionNotificationModes(
				FileHandle,
				Flags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileCompletionNotificationModes));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileSecurityA(
				lpFileName,
				SecurityInformation,
				pSecurityDescriptor);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileSecurityA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileShortNameA(
				hFile,
				lpShortName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileShortNameA));
			message.AppendValue(lpShortName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileShortNameW(
				hFile,
				lpShortName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileShortNameW));
			message.AppendValue(lpShortName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetSearchPathMode(
		DWORD Flags)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetSearchPathMode(
				Flags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetSearchPathMode));
			message.AppendValue(Flags);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::WriteEncryptedFileRaw(
				pfImportCallback,
				pvCallbackContext,
				pvContext);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::WriteEncryptedFileRaw));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}
}