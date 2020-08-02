#pragma once
#include "WinBaseCache.h"

namespace Functions::WinBase::Overrides
{
	BOOL WINAPI CopyFileA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		BOOL bFailIfExists)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CopyFileA(
				lpExistingFileName,
				lpNewFileName,
				bFailIfExists);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CopyFileA;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, bFailIfExists);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CopyFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		BOOL bFailIfExists)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CopyFileW(
				lpExistingFileName,
				lpNewFileName,
				bFailIfExists);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CopyFileW;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, bFailIfExists);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	HRESULT WINAPI CopyFile2(
		PCWSTR pwszExistingFileName,
		PCWSTR pwszNewFileName,
		COPYFILE2_EXTENDED_PARAMETERS *pExtendedParameters)
	{
		HRESULT result = 0;
		__try
		{
			result = Cache::CopyFile2(
				pwszExistingFileName,
				pwszNewFileName,
				pExtendedParameters);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CopyFile2;
			EventLogger::AppendValue(message, pwszExistingFileName);
			EventLogger::AppendValue(message, pwszNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
		{
			result = Cache::CopyFileExA(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CopyFileExA;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
		{
			result = Cache::CopyFileExW(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CopyFileExW;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
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
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CopyFileTransactedA;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
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
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CopyFileTransactedW;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryExA(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CreateDirectoryExA(
				lpTemplateDirectory,
				lpNewDirectory,
				lpSecurityAttributes);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateDirectoryExA;
			EventLogger::AppendValue(message, lpTemplateDirectory);
			EventLogger::AppendValue(message, lpNewDirectory);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryExW(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CreateDirectoryExW(
				lpTemplateDirectory,
				lpNewDirectory,
				lpSecurityAttributes);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateDirectoryExW;
			EventLogger::AppendValue(message, lpTemplateDirectory);
			EventLogger::AppendValue(message, lpNewDirectory);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryTransactedA(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CreateDirectoryTransactedA(
				lpTemplateDirectory,
				lpNewDirectory,
				lpSecurityAttributes,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateDirectoryTransactedA;
			EventLogger::AppendValue(message, lpTemplateDirectory);
			EventLogger::AppendValue(message, lpNewDirectory);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryTransactedW(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CreateDirectoryTransactedW(
				lpTemplateDirectory,
				lpNewDirectory,
				lpSecurityAttributes,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateDirectoryTransactedW;
			EventLogger::AppendValue(message, lpTemplateDirectory);
			EventLogger::AppendValue(message, lpNewDirectory);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		HANDLE result = 0;
		__try
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
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateFileTransactedA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwDesiredAccess);
			EventLogger::AppendValue(message, dwShareMode);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		HANDLE result = 0;
		__try
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
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateFileTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwDesiredAccess);
			EventLogger::AppendValue(message, dwShareMode);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateHardLinkA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateHardLinkW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateHardLinkTransactedA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateHardLinkTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateProcessWithLogonW;
			EventLogger::AppendValue(message, lpApplicationName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);

			// TODO
			EventLogger::WriteError("Cannot detour process from CreateProcessWithLogonW");
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateProcessWithTokenW;
			EventLogger::AppendValue(message, lpApplicationName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);

			// TODO
			EventLogger::WriteError("Cannot detour process from CreateProcessWithTokenW");
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateSymbolicLinkA;
			EventLogger::AppendValue(message, lpSymlinkFileName);
			EventLogger::AppendValue(message, lpTargetFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateSymbolicLinkW;
			EventLogger::AppendValue(message, lpSymlinkFileName);
			EventLogger::AppendValue(message, lpTargetFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateSymbolicLinkTransactedA;
			EventLogger::AppendValue(message, lpSymlinkFileName);
			EventLogger::AppendValue(message, lpTargetFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}
	BOOLEAN WINAPI CreateSymbolicLinkTransactedW(
		LPCWSTR lpSymlinkFileName,
		LPCWSTR lpTargetFileName,
		DWORD dwFlags,
		HANDLE hTransaction)
	{
		BOOL result = 0;
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateSymbolicLinkTransactedW;
			EventLogger::AppendValue(message, lpSymlinkFileName);
			EventLogger::AppendValue(message, lpTargetFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DecryptFileA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DecryptFileW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DeleteFile;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileTransactedA(
		LPCSTR lpFileName,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::DeleteFileTransactedA(
				lpFileName,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DeleteFileTransactedA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileTransactedW(
		LPCWSTR lpFileName,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::DeleteFileTransactedW(
				lpFileName,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DeleteFileTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::EncryptFileA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::EncryptFileW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FileEncryptionStatusA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FileEncryptionStatusW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstFileNameTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstFileTransactedA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstFileTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstStreamTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetBinaryTypeA;
			EventLogger::AppendValue(message, lpApplicationName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetBinaryTypeW;
			EventLogger::AppendValue(message, lpApplicationName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetCompressedFileSizeTransactedA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetCompressedFileSizeTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetDllDirectoryA;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetDllDirectoryW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesTransactedA(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::GetFileAttributesTransactedA(
				lpFileName,
				fInfoLevelId,
				lpFileInformation,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileAttributesTransactedA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesTransactedW(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::GetFileAttributesTransactedW(
				lpFileName,
				fInfoLevelId,
				lpFileInformation,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileAttributesTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileBandwidthReservation;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileInformationByHandleEx;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
		{
			result = Cache::GetFileSecurityA(
				lpFileName,
				RequestedInformation,
				pSecurityDescriptor,
				nLength,
				lpnLengthNeeded);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileSecurityA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFullPathNameTransactedA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFullPathNameTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetLongPathNameTransactedA;
			EventLogger::AppendValue(message, lpszShortPath);
			EventLogger::AppendValue(message, lpszLongPath);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetLongPathNameTransactedW;
			EventLogger::AppendValue(message, lpszShortPath);
			EventLogger::AppendValue(message, lpszLongPath);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetQueuedCompletionStatus;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetQueuedCompletionStatusEx;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetShortPathNameA;
			EventLogger::AppendValue(message, lpszLongPath);
			EventLogger::AppendValue(message, lpszShortPath);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LoadModule;
			EventLogger::AppendValue(message, lpModuleName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LoadPackagedLibrary;
			EventLogger::AppendValue(message, lpwLibFileName);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileA(
		LPCTSTR lpExistingFileName,
		LPCTSTR lpNewFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::MoveFileA(
				lpExistingFileName,
				lpNewFileName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::MoveFileA;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::MoveFileW(
				lpExistingFileName,
				lpNewFileName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::MoveFileW;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		DWORD dwFlags)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::MoveFileExA(
				lpExistingFileName,
				lpNewFileName,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::MoveFileExA;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI MoveFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		DWORD dwFlags)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::MoveFileExW(
				lpExistingFileName,
				lpNewFileName,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::MoveFileExW;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
		{
			result = Cache::MoveFileTransactedA(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				dwFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::MoveFileTransactedA;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
		{
			result = Cache::MoveFileTransactedW(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				dwFlags,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::MoveFileTransactedW;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
		{
			result = Cache::MoveFileWithProgressA(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::MoveFileWithProgressA;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		BOOL result = 0;
		__try
		{
			result = Cache::MoveFileWithProgressW(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::MoveFileWithProgressW;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::OpenEncryptedFileRawA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, ulFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::OpenEncryptedFileRawW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, ulFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	HFILE WINAPI OpenFile(
		LPCSTR lpFileName,
		LPOFSTRUCT lpReOpenBuff,
		UINT uStyle)
	{
		HFILE result = 0;
		__try
		{
			result = Cache::OpenFile(
				lpFileName,
				lpReOpenBuff,
				uStyle);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::OpenFile;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::OpenFileById;
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::ReadEncryptedFileRaw;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryTransactedA(
		LPCSTR lpPathName,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::RemoveDirectoryTransactedA(
				lpPathName,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::RemoveDirectoryTransactedA;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryTransactedW(
		LPCWSTR lpPathName,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::RemoveDirectoryTransactedW(
				lpPathName,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::RemoveDirectoryTransactedW;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::ReOpenFile;
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::ReplaceFileA;
			EventLogger::AppendValue(message, lpReplacedFileName);
			EventLogger::AppendValue(message, lpReplacementFileName);
			EventLogger::AppendValue(message, lpBackupFileName);
			EventLogger::AppendValue(message, dwReplaceFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::ReplaceFileW;
			EventLogger::AppendValue(message, lpReplacedFileName);
			EventLogger::AppendValue(message, lpReplacementFileName);
			EventLogger::AppendValue(message, lpBackupFileName);
			EventLogger::AppendValue(message, dwReplaceFlags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetCurrentDirectoryA;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetCurrentDirectoryW;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetDllDirectoryA;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetDllDirectoryW;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesTransactedA(
		LPCSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetFileAttributesTransactedA(
				lpFileName,
				dwFileAttributes,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileAttributesTransactedA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwFileAttributes);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesTransactedW(
		LPCWSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetFileAttributesTransactedW(
				lpFileName,
				dwFileAttributes,
				hTransaction);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileAttributesTransactedW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwFileAttributes);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileBandwidthReservation;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileCompletionNotificationModes;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileSecurityA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileShortNameA;
			EventLogger::AppendValue(message, lpShortName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileShortNameW;
			EventLogger::AppendValue(message, lpShortName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetSearchPathMode;
			EventLogger::AppendValue(message, Flags);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::WriteEncryptedFileRaw;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}
}