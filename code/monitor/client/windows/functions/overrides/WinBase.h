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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileA));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(bFailIfExists);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI CopyFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		BOOL bFailIfExists)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileW));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(bFailIfExists);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	HRESULT WINAPI CopyFile2(
		PCWSTR pwszExistingFileName,
		PCWSTR pwszNewFileName,
		COPYFILE2_EXTENDED_PARAMETERS *pExtendedParameters)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFile2));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(pwszExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(pwszNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		HRESULT result = 0;
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

		message.AppendValue(pwszExistingFileName);
		message.AppendValue(pwszNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileExA));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileExW));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileTransactedA));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CopyFileTransactedW));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI CreateDirectoryExA(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryExA));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
		BOOL result = 0;
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

		message.AppendValue(lpTemplateDirectory);
		message.AppendValue(lpNewDirectory);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI CreateDirectoryExW(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryExW));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
		BOOL result = 0;
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

		message.AppendValue(lpTemplateDirectory);
		message.AppendValue(lpNewDirectory);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI CreateDirectoryTransactedA(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryTransactedA));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
		BOOL result = 0;
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

		message.AppendValue(lpTemplateDirectory);
		message.AppendValue(lpNewDirectory);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI CreateDirectoryTransactedW(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryTransactedW));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewDirectory);
		BOOL result = 0;
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

		message.AppendValue(lpTemplateDirectory);
		message.AppendValue(lpNewDirectory);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFileTransactedA));

		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(dwDesiredAccess);
		message.AppendValue(dwShareMode);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFileTransactedW));

		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(dwDesiredAccess);
		message.AppendValue(dwShareMode);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI CreateHardLinkA(
		LPCSTR lpFileName,
		LPCSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateHardLinkA));

		BOOL result = Cache::WinBase::CreateHardLinkA(
			lpFileName,
			lpExistingFileName,
			lpSecurityAttributes);

		message.AppendValue(lpFileName);
		message.AppendValue(lpExistingFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI CreateHardLinkW(
		LPCWSTR lpFileName,
		LPCWSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateHardLinkW));

		BOOL result = Cache::WinBase::CreateHardLinkW(
			lpFileName,
			lpExistingFileName,
			lpSecurityAttributes);;

		message.AppendValue(lpFileName);
		message.AppendValue(lpExistingFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI CreateHardLinkTransactedA(
		LPCSTR lpFileName,
		LPCSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateHardLinkTransactedA));

		BOOL result = Cache::WinBase::CreateHardLinkTransactedA(
			lpFileName,
			lpExistingFileName,
			lpSecurityAttributes,
			hTransaction);

		message.AppendValue(lpFileName);
		message.AppendValue(lpExistingFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI CreateHardLinkTransactedW(
		LPCWSTR lpFileName,
		LPCWSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateHardLinkTransactedW));

		BOOL result = Cache::WinBase::CreateHardLinkTransactedW(
			lpFileName,
			lpExistingFileName,
			lpSecurityAttributes,
			hTransaction);

		message.AppendValue(lpFileName);
		message.AppendValue(lpExistingFileName);
		message.AppendValue(result);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateProcessWithLogonW));

		BOOL result = Cache::WinBase::CreateProcessWithLogonW(
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

		message.AppendValue(lpApplicationName);
		message.AppendValue(result);

		// TODO
		{
			auto errorMessage = MessageSender(MessageType::Error);
			errorMessage.AppendValue("Cannot detour process from CreateProcessWithLogonW");
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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateProcessWithTokenW));

		BOOL result = Cache::WinBase::CreateProcessWithTokenW(
			hToken,
			dwLogonFlags,
			lpApplicationName,
			lpCommandLine,
			dwCreationFlags,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			lpProcessInformation);

		message.AppendValue(lpApplicationName);
		message.AppendValue(result);

		// TODO
		{
			auto errorMessage = MessageSender(MessageType::Error);
			errorMessage.AppendValue("Cannot detour process from CreateProcessWithTokenW");
		}

		return result;
	}

	BOOLEAN WINAPI CreateSymbolicLinkA(
		LPCSTR lpSymlinkFileName,
		LPCSTR lpTargetFileName,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateSymbolicLinkA));

		BOOLEAN result = Cache::WinBase::CreateSymbolicLinkA(
			lpSymlinkFileName,
			lpTargetFileName,
			dwFlags);

		message.AppendValue(lpSymlinkFileName);
		message.AppendValue(lpTargetFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);

		return result;
	}

	BOOLEAN WINAPI CreateSymbolicLinkW(
		LPCWSTR lpSymlinkFileName,
		LPCWSTR lpTargetFileName,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateSymbolicLinkW));

		BOOLEAN result = Cache::WinBase::CreateSymbolicLinkW(
			lpSymlinkFileName,
			lpTargetFileName,
			dwFlags);

		message.AppendValue(lpSymlinkFileName);
		message.AppendValue(lpTargetFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);

		return result;
	}

	BOOLEAN WINAPI CreateSymbolicLinkTransactedA(
		LPCSTR lpSymlinkFileName,
		LPCSTR lpTargetFileName,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateSymbolicLinkTransactedA));

		BOOLEAN result = Cache::WinBase::CreateSymbolicLinkTransactedA(
			lpSymlinkFileName,
			lpTargetFileName,
			dwFlags,
			hTransaction);

		message.AppendValue(lpSymlinkFileName);
		message.AppendValue(lpTargetFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);

		return result;
	}

	BOOLEAN WINAPI CreateSymbolicLinkTransactedW(
		LPCWSTR lpSymlinkFileName,
		LPCWSTR lpTargetFileName,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateSymbolicLinkTransactedW));

		BOOLEAN result = Cache::WinBase::CreateSymbolicLinkTransactedW(
			lpSymlinkFileName,
			lpTargetFileName,
			dwFlags,
			hTransaction);

		message.AppendValue(lpSymlinkFileName);
		message.AppendValue(lpTargetFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI DecryptFileA(
		LPCSTR lpFileName,
		DWORD dwReserved)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DecryptFileA));

		BOOL result = Cache::WinBase::DecryptFileA(lpFileName, dwReserved);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI DecryptFileW(
		LPCWSTR lpFileName,
		DWORD dwReserved)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DecryptFileW));

		BOOL result = Cache::WinBase::DecryptFileW(lpFileName, dwReserved);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI DeleteFile(
		LPCSTR lpFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFile));

		BOOL result = Cache::WinBase::DeleteFile(lpFileName);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI DeleteFileTransactedA(
		LPCSTR lpFileName,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFileTransactedA));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		BOOL result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI DeleteFileTransactedW(
		LPCWSTR lpFileName,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFileTransactedW));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		BOOL result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI EncryptFileA(
		LPCSTR lpFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::EncryptFileA));

		BOOL result = Cache::WinBase::EncryptFileA(lpFileName);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI EncryptFileW(
		LPCWSTR lpFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::EncryptFileW));

		BOOL result = Cache::WinBase::EncryptFileW(lpFileName);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FileEncryptionStatusA(
		LPCSTR lpFileName,
		LPDWORD lpStatus)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FileEncryptionStatusA));

		BOOL result = Cache::WinBase::FileEncryptionStatusA(lpFileName, lpStatus);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FileEncryptionStatusW(
		LPCWSTR lpFileName,
		LPDWORD lpStatus)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FileEncryptionStatusW));

		BOOL result = Cache::WinBase::FileEncryptionStatusW(lpFileName, lpStatus);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	HANDLE WINAPI FindFirstFileNameTransactedW(
		LPCWSTR lpFileName,
		DWORD dwFlags,
		LPDWORD StringLength,
		PWSTR LinkName,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileNameTransactedW));

		HANDLE result = Cache::WinBase::FindFirstFileNameTransactedW(
			lpFileName,
			dwFlags,
			StringLength,
			LinkName,
			hTransaction);

		message.AppendValue(lpFileName);
		message.AppendValue(dwFlags);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileTransactedA));

		HANDLE result = Cache::WinBase::FindFirstFileTransactedA(
			lpFileName,
			fInfoLevelId,
			lpFindFileData,
			fSearchOp,
			lpSearchFilter,
			dwAdditionalFlags,
			hTransaction);

		message.AppendValue(lpFileName);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileTransactedW));

		HANDLE result = Cache::WinBase::FindFirstFileTransactedW(
			lpFileName,
			fInfoLevelId,
			lpFindFileData,
			fSearchOp,
			lpSearchFilter,
			dwAdditionalFlags,
			hTransaction);

		message.AppendValue(lpFileName);

		return result;
	}

	HANDLE WINAPI FindFirstStreamTransactedW(
		LPCWSTR lpFileName,
		STREAM_INFO_LEVELS InfoLevel,
		LPVOID lpFindStreamData,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstStreamTransactedW));

		HANDLE result = Cache::WinBase::FindFirstStreamTransactedW(
			lpFileName,
			InfoLevel,
			lpFindStreamData,
			dwFlags,
			hTransaction);

		message.AppendValue(lpFileName);

		return result;
	}

	BOOL WINAPI GetBinaryTypeA(
		LPCSTR lpApplicationName,
		LPDWORD lpBinaryType)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetBinaryTypeA));

		BOOL result = Cache::WinBase::GetBinaryTypeA(lpApplicationName, lpBinaryType);

		message.AppendValue(lpApplicationName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetBinaryTypeW(
		LPCWSTR lpApplicationName,
		LPDWORD lpBinaryType)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetBinaryTypeW));

		BOOL result = Cache::WinBase::GetBinaryTypeW(lpApplicationName, lpBinaryType);

		message.AppendValue(lpApplicationName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetCompressedFileSizeTransactedA(
		LPCSTR lpFileName,
		LPDWORD lpFileSizeHigh,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetCompressedFileSizeTransactedA));

		DWORD result = Cache::WinBase::GetCompressedFileSizeTransactedA(
			lpFileName,
			lpFileSizeHigh,
			hTransaction);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetCompressedFileSizeTransactedW(
		LPCWSTR lpFileName,
		LPDWORD lpFileSizeHigh,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetCompressedFileSizeTransactedW));

		DWORD result = Cache::WinBase::GetCompressedFileSizeTransactedW(
			lpFileName,
			lpFileSizeHigh,
			hTransaction);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetDllDirectoryA(
		DWORD nBufferLength,
		LPSTR lpBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDllDirectoryA));

		DWORD result = Cache::WinBase::GetDllDirectoryA(
			nBufferLength,
			lpBuffer);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetDllDirectoryW(
		DWORD nBufferLength,
		LPWSTR lpBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDllDirectoryW));

		BOOL result = Cache::WinBase::GetDllDirectoryW(nBufferLength, lpBuffer);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetEnvironmentVariableA(
		LPCTSTR lpName,
		LPTSTR lpBuffer,
		DWORD nSize)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetEnvironmentVariableA));

		DWORD result = Cache::WinBase::GetEnvironmentVariableA(
			lpName,
			lpBuffer,
			nSize);

		message.AppendValue(lpName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetEnvironmentVariableW(
		LPCWSTR lpName,
		LPWSTR lpBuffer,
		DWORD nSize)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetEnvironmentVariableW));

		DWORD result = Cache::WinBase::GetEnvironmentVariableW(
			lpName,
			lpBuffer,
			nSize);

		message.AppendValue(lpName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetFileAttributesTransactedA(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesTransactedA));

		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		BOOL result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI GetFileAttributesTransactedW(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesTransactedW));

		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		BOOL result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileBandwidthReservation));

		BOOL result = Cache::WinBase::GetFileBandwidthReservation(
			hFile,
			lpPeriodMilliseconds,
			lpBytesPerPeriod,
			pDiscardable,
			lpTransferSize,
			lpNumOutstandingRequests);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetFileInformationByHandleEx(
		HANDLE hFile,
		FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
		LPVOID lpFileInformation,
		DWORD dwBufferSize)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileInformationByHandleEx));

		BOOL result = Cache::WinBase::GetFileInformationByHandleEx(
			hFile,
			FileInformationClass,
			lpFileInformation,
			dwBufferSize);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetFileSecurityA(
		LPCSTR lpFileName,
		SECURITY_INFORMATION RequestedInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor,
		DWORD nLength,
		LPDWORD lpnLengthNeeded)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileSecurityA));

		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		BOOL result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	DWORD WINAPI GetFullPathNameTransactedA(
		LPCSTR lpFileName,
		DWORD nBufferLength,
		LPSTR lpBuffer,
		LPSTR *lpFilePart,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFullPathNameTransactedA));

		DWORD result = Cache::WinBase::GetFullPathNameTransactedA(
			lpFileName,
			nBufferLength,
			lpBuffer,
			lpFilePart,
			hTransaction);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetFullPathNameTransactedW(
		LPCWSTR lpFileName,
		DWORD nBufferLength,
		LPWSTR lpBuffer,
		LPWSTR *lpFilePart,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFullPathNameTransactedW));

		DWORD result = Cache::WinBase::GetFullPathNameTransactedW(
			lpFileName,
			nBufferLength,
			lpBuffer,
			lpFilePart,
			hTransaction);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetLongPathNameTransactedA(
		LPCSTR lpszShortPath,
		LPSTR lpszLongPath,
		DWORD cchBuffer,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLongPathNameTransactedA));

		DWORD result = Cache::WinBase::GetLongPathNameTransactedA(
			lpszShortPath,
			lpszLongPath,
			cchBuffer,
			hTransaction);

		message.AppendValue(lpszShortPath);
		message.AppendValue(lpszLongPath);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetLongPathNameTransactedW(
		LPCWSTR lpszShortPath,
		LPWSTR lpszLongPath,
		DWORD cchBuffer,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLongPathNameTransactedW));

		DWORD result = Cache::WinBase::GetLongPathNameTransactedW(
			lpszShortPath,
			lpszLongPath,
			cchBuffer,
			hTransaction);

		message.AppendValue(lpszShortPath);
		message.AppendValue(lpszLongPath);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetQueuedCompletionStatus(
		HANDLE CompletionPort,
		LPDWORD lpNumberOfBytesTransferred,
		PULONG_PTR lpCompletionKey,
		LPOVERLAPPED *lpOverlapped,
		DWORD dwMilliseconds)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetQueuedCompletionStatus));

		BOOL result = Cache::WinBase::GetQueuedCompletionStatus(
			CompletionPort,
			lpNumberOfBytesTransferred,
			lpCompletionKey,
			lpOverlapped,
			dwMilliseconds);

		message.AppendValue(result);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetQueuedCompletionStatusEx));

		BOOL result = Cache::WinBase::GetQueuedCompletionStatusEx(
			CompletionPort,
			lpCompletionPortEntries,
			ulCount,
			ulNumEntriesRemoved,
			dwMilliseconds,
			fAlertable);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetShortPathNameA(
		LPCSTR lpszLongPath,
		LPSTR lpszShortPath,
		DWORD cchBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetShortPathNameA));

		BOOL result = Cache::WinBase::GetShortPathNameA(
			lpszLongPath,
			lpszShortPath,
			cchBuffer);

		message.AppendValue(lpszLongPath);
		message.AppendValue(lpszShortPath);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI LoadModule(
		LPCSTR lpModuleName,
		LPVOID lpParameterBlock)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadModule));

		DWORD result = Cache::WinBase::LoadModule(lpModuleName, lpParameterBlock);

		message.AppendValue(lpModuleName);
		message.AppendValue(result);

		return result;
	}

	HMODULE WINAPI LoadPackagedLibrary(
		LPCWSTR lpwLibFileName,
		DWORD Reserved)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadPackagedLibrary));

		HMODULE result = Cache::WinBase::LoadPackagedLibrary(lpwLibFileName, Reserved);

		message.AppendValue(lpwLibFileName);

		return result;
	}

	BOOL WINAPI MoveFileA(
		LPCTSTR lpExistingFileName,
		LPCTSTR lpNewFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileA));

		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI MoveFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileW));

		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI MoveFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileExA));

		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI MoveFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileExW));

		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileTransactedA));

		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileTransactedW));

		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI MoveFileWithProgressA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileWithProgressA));

		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI MoveFileWithProgressW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::MoveFileWithProgressW));

		// Check if this file is allowed write access
		bool blockExistingFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpExistingFileName);
		bool blockNewFileAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockExistingFileAccess || blockNewFileAccess;
		BOOL result = 0;
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

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(dwFlags);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	DWORD WINAPI OpenEncryptedFileRawA(
		LPCSTR lpFileName,
		ULONG ulFlags,
		PVOID *pvContext)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::OpenEncryptedFileRawA));

		DWORD result = Cache::WinBase::OpenEncryptedFileRawA(
			lpFileName,
			ulFlags,
			pvContext);

		message.AppendValue(lpFileName);
		message.AppendValue(ulFlags);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI OpenEncryptedFileRawW(
		LPCWSTR lpFileName,
		ULONG ulFlags,
		PVOID *pvContext)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::OpenEncryptedFileRawW));

		DWORD result = Cache::WinBase::OpenEncryptedFileRawW(
			lpFileName,
			ulFlags,
			pvContext);

		message.AppendValue(lpFileName);
		message.AppendValue(ulFlags);
		message.AppendValue(result);

		return result;
	}

	HFILE WINAPI OpenFile(
		LPCSTR lpFileName,
		LPOFSTRUCT lpReOpenBuff,
		UINT uStyle)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::OpenFile));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		HFILE result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::OpenFileById));

		HANDLE result = Cache::WinBase::OpenFileById(
			hVolumeHint,
			lpFileId,
			dwDesiredAccess,
			dwShareMode,
			lpSecurityAttributes,
			dwFlagsAndAttributes);

		return result;
	}

	DWORD WINAPI ReadEncryptedFileRaw(
		PFE_EXPORT_FUNC pfExportCallback,
		PVOID pvCallbackContext,
		PVOID pvContext)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::ReadEncryptedFileRaw));

		DWORD result = Cache::WinBase::ReadEncryptedFileRaw(
			pfExportCallback,
			pvCallbackContext,
			pvContext);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI RemoveDirectoryTransactedA(
		LPCSTR lpPathName,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::RemoveDirectoryTransactedA));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpPathName);
		BOOL result = 0;
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

		message.AppendValue(lpPathName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI RemoveDirectoryTransactedW(
		LPCWSTR lpPathName,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::RemoveDirectoryTransactedW));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpPathName);
		BOOL result = 0;
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

		message.AppendValue(lpPathName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	HANDLE WINAPI ReOpenFile(
		HANDLE hOriginalFile,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwFlagsAndAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::ReOpenFile));

		HANDLE result = Cache::WinBase::ReOpenFile(
			hOriginalFile,
			dwDesiredAccess,
			dwShareMode,
			dwFlagsAndAttributes);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::ReplaceFileA));

		BOOL result = Cache::WinBase::ReplaceFileA(
			lpReplacedFileName,
			lpReplacementFileName,
			lpBackupFileName,
			dwReplaceFlags,
			lpExclude,
			lpReserved);

		message.AppendValue(lpReplacedFileName);
		message.AppendValue(lpReplacementFileName);
		message.AppendValue(lpBackupFileName);
		message.AppendValue(dwReplaceFlags);
		message.AppendValue(result);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::ReplaceFileW));

		BOOL result = Cache::WinBase::ReplaceFileW(
			lpReplacedFileName,
			lpReplacementFileName,
			lpBackupFileName,
			dwReplaceFlags,
			lpExclude,
			lpReserved);

		message.AppendValue(lpReplacedFileName);
		message.AppendValue(lpReplacementFileName);
		message.AppendValue(lpBackupFileName);
		message.AppendValue(dwReplaceFlags);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetCurrentDirectoryA(
		LPCTSTR lpPathName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetCurrentDirectoryA));

		BOOL result = Cache::WinBase::SetCurrentDirectoryA(lpPathName);

		if (result)
		{
			FileSystemAccessSandbox::UpdateWorkingDirectory(lpPathName);
		}

		message.AppendValue(lpPathName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetCurrentDirectoryW(
		LPCWSTR lpPathName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetCurrentDirectoryW));

		BOOL result = Cache::WinBase::SetCurrentDirectoryW(lpPathName);

		if (result)
		{
			FileSystemAccessSandbox::UpdateWorkingDirectory(lpPathName);
		}

		message.AppendValue(lpPathName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetDllDirectoryA(
		LPCSTR lpPathName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetDllDirectoryA));

		BOOL result = Cache::WinBase::SetDllDirectoryA(lpPathName);

		message.AppendValue(lpPathName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetDllDirectoryW(
		LPCWSTR lpPathName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetDllDirectoryW));

		BOOL result = Cache::WinBase::SetDllDirectoryW(lpPathName);

		message.AppendValue(lpPathName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFileAttributesTransactedA(
		LPCSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileAttributesTransactedA));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		BOOL result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(dwFileAttributes);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI SetFileAttributesTransactedW(
		LPCWSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileAttributesTransactedW));

		// Check if this file is allowed write access
		bool blockAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpFileName);
		BOOL result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(dwFileAttributes);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileBandwidthReservation));

		BOOL result = Cache::WinBase::SetFileBandwidthReservation(
			hFile,
			nPeriodMilliseconds,
			nBytesPerPeriod,
			bDiscardable,
			lpTransferSize,
			lpNumOutstandingRequests);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFileCompletionNotificationModes(
		HANDLE FileHandle,
		UCHAR Flags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileCompletionNotificationModes));

		BOOL result = Cache::WinBase::SetFileCompletionNotificationModes(FileHandle, Flags);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFileSecurityA(
		LPCSTR lpFileName,
		SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileSecurityA));

		BOOL result = Cache::WinBase::SetFileSecurityA(
			lpFileName,
			SecurityInformation,
			pSecurityDescriptor);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFileShortNameA(
		HANDLE hFile,
		LPCSTR lpShortName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileShortNameA));

		BOOL result = Cache::WinBase::SetFileShortNameA(hFile, lpShortName);

		message.AppendValue(lpShortName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFileShortNameW(
		HANDLE hFile,
		LPCWSTR lpShortName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileShortNameW));

		BOOL result = Cache::WinBase::SetFileShortNameW(hFile, lpShortName);

		message.AppendValue(lpShortName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetSearchPathMode(
		DWORD Flags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetSearchPathMode));

		BOOL result = Cache::WinBase::SetSearchPathMode(Flags);

		message.AppendValue(Flags);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI WriteEncryptedFileRaw(
		PFE_IMPORT_FUNC pfImportCallback,
		PVOID pvCallbackContext,
		PVOID pvContext)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::WriteEncryptedFileRaw));

		BOOL result = Cache::WinBase::WriteEncryptedFileRaw(
			pfImportCallback,
			pvCallbackContext,
			pvContext);

		message.AppendValue(result);

		return result;
	}
}