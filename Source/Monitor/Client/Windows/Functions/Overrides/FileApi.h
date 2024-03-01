#pragma once
#include "../Cache/FileApi.h"
#include "WindowsConnectionManager.h"
#include "FileSystemAccessSandbox.h"

namespace Monitor::Windows::Functions::Overrides::FileApi
{
	BOOL WINAPI AreFileApisANSI()
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::AreFileApisANSI();
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::AreFileApisANSI));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	LONG WINAPI CompareFileTime(
		const FILETIME *lpFileTime1,
		const FILETIME *lpFileTime2)
	{
		LONG result = 0;
		__try
		{
			result = Cache::FileApi::CompareFileTime(
				lpFileTime1,
				lpFileTime2
			);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CompareFileTime));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryA(
		LPCSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
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
				result = Cache::FileApi::CreateDirectoryA(
					lpPathName,
					lpSecurityAttributes);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryW(
		LPCWSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
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
				result = Cache::FileApi::CreateDirectoryW(
					lpPathName,
					lpSecurityAttributes);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI CreateFile2(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams)
	{
		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;
		__try
		{
			if (blockAccess)
			{
				result = INVALID_HANDLE_VALUE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::FileApi::CreateFile2(
					lpFileName,
					dwDesiredAccess,
					dwShareMode,
					dwCreationDisposition,
					pCreateExParams);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFile2));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(dwCreationDisposition);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI CreateFileA(
		LPCSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile)
	{
		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;
		__try
		{
			if (blockAccess)
			{
				result = INVALID_HANDLE_VALUE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::FileApi::CreateFileA(
					lpFileName,
					dwDesiredAccess,
					dwShareMode,
					lpSecurityAttributes,
					dwCreationDisposition,
					dwFlagsAndAttributes,
					hTemplateFile);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFileA));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(dwCreationDisposition);
			message.AppendValue(dwFlagsAndAttributes);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI CreateFileW(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile)
	{
		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;
		__try
		{
			if (blockAccess)
			{
				result = INVALID_HANDLE_VALUE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::FileApi::CreateFileW(
					lpFileName,
					dwDesiredAccess,
					dwShareMode,
					lpSecurityAttributes,
					dwCreationDisposition,
					dwFlagsAndAttributes,
					hTemplateFile);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFileW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(dwCreationDisposition);
			message.AppendValue(dwFlagsAndAttributes);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DefineDosDeviceW(
		DWORD dwFlags,
		LPCWSTR lpDeviceName,
		LPCWSTR lpTargetPath)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::DefineDosDeviceW(
				dwFlags,
				lpDeviceName,
				lpTargetPath);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DefineDosDeviceW));
			message.AppendValue(dwFlags);
			message.AppendValue(lpDeviceName);
			message.AppendValue(lpTargetPath);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileA(
		LPCSTR lpFileName)
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
				result = Cache::FileApi::DeleteFileA(
					lpFileName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFileA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileW(
		LPCWSTR lpFileName)
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
				result = Cache::FileApi::DeleteFileW(
					lpFileName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFileW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteVolumeMountPointW(
		LPCWSTR lpszVolumeMountPoint)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::DeleteVolumeMountPointW(
				lpszVolumeMountPoint);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteVolumeMountPointW));
			message.AppendValue(lpszVolumeMountPoint);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FileTimeToLocalFileTime(
		const FILETIME *lpFileTime,
		LPFILETIME lpLocalFileTime)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FileTimeToLocalFileTime(
				lpFileTime,
				lpLocalFileTime
			);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FileTimeToLocalFileTime));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindClose(
		HANDLE hFindFile)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindClose(
				hFindFile);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindClose));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindCloseChangeNotification(
		HANDLE hChangeHandle)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindCloseChangeNotification(
				hChangeHandle);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindCloseChangeNotification));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstChangeNotificationA(
		LPCSTR lpPathName,
		BOOL bWatchSubtree,
		DWORD dwNotifyFilter)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstChangeNotificationA(
				lpPathName,
				bWatchSubtree,
				dwNotifyFilter);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstChangeNotificationA));
			message.AppendValue(lpPathName);
			message.AppendValue(bWatchSubtree);
			message.AppendValue(dwNotifyFilter);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstChangeNotificationW(
		LPCWSTR lpPathName,
		BOOL bWatchSubtree,
		DWORD dwNotifyFilter)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstChangeNotificationW(
				lpPathName,
				bWatchSubtree,
				dwNotifyFilter);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstChangeNotificationW));
			message.AppendValue(lpPathName);
			message.AppendValue(bWatchSubtree);
			message.AppendValue(dwNotifyFilter);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstFileA(
		LPCSTR lpFileName,
		LPWIN32_FIND_DATAA lpFindFileData)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstFileA(
				lpFileName,
				lpFindFileData);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileA));
			message.AppendValue(lpFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstFileW(
		LPCWSTR lpFileName,
		LPWIN32_FIND_DATAW lpFindFileData)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstFileW(
				lpFileName,
				lpFindFileData);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileW));
			message.AppendValue(lpFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstFileExA(
		LPCSTR lpFileName,
		FINDEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFindFileData,
		FINDEX_SEARCH_OPS fSearchOp,
		LPVOID lpSearchFilter,
		DWORD dwAdditionalFlags)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstFileExA(
				lpFileName,
				fInfoLevelId,
				lpFindFileData,
				fSearchOp,
				lpSearchFilter,
				dwAdditionalFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileExA));
			message.AppendValue(lpFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstFileExW(
		LPCWSTR lpFileName,
		FINDEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFindFileData,
		FINDEX_SEARCH_OPS fSearchOp,
		LPVOID lpSearchFilter,
		DWORD dwAdditionalFlags)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstFileExW(
				lpFileName,
				fInfoLevelId,
				lpFindFileData,
				fSearchOp,
				lpSearchFilter,
				dwAdditionalFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileExW));
			message.AppendValue(lpFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstFileNameW(
		LPCWSTR lpFileName,
		DWORD dwFlags,
		LPDWORD StringLength,
		PWSTR LinkName)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstFileNameW(
				lpFileName,
				dwFlags,
				StringLength,
				LinkName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileNameW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwFlags);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstStreamW(
		LPCWSTR lpFileName,
		STREAM_INFO_LEVELS InfoLevel,
		LPVOID lpFindStreamData,
		DWORD dwFlags)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstStreamW(
				lpFileName,
				InfoLevel,
				lpFindStreamData,
				dwFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstStreamW));
			message.AppendValue(lpFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HANDLE WINAPI FindFirstVolumeW(
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength)
	{
		HANDLE result = 0;
		__try
		{
			result = Cache::FileApi::FindFirstVolumeW(
				lpszVolumeName,
				cchBufferLength);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstVolumeW));
			message.AppendValue(lpszVolumeName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindNextChangeNotification(
		HANDLE hChangeHandle)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindNextChangeNotification(
				hChangeHandle);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextChangeNotification));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindNextFileA(
		HANDLE hFindFile,
		LPWIN32_FIND_DATAA lpFindFileData)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindNextFileA(
				hFindFile,
				lpFindFileData);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextFileA));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindNextFileW(
		HANDLE hFindFile,
		LPWIN32_FIND_DATAW lpFindFileData)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindNextFileW(
				hFindFile,
				lpFindFileData);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextFileW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindNextFileNameW(
		HANDLE hFindStream,
		LPDWORD StringLength,
		PWSTR LinkName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindNextFileNameW(
				hFindStream,
				StringLength,
				LinkName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextFileNameW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindNextStreamW(
		HANDLE hFindStream,
		LPVOID lpFindStreamData)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindNextStreamW(
				hFindStream,
				lpFindStreamData);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextStreamW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindNextVolumeW(
		HANDLE hFindVolume,
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindNextVolumeW(
				hFindVolume,
				lpszVolumeName,
				cchBufferLength);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextVolumeW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindVolumeClose(
		HANDLE hFindVolume)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FindVolumeClose(
				hFindVolume);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FindVolumeClose));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FlushFileBuffers(
		HANDLE hFile)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::FlushFileBuffers(
				hFile);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::FlushFileBuffers));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetCompressedFileSizeA(
		LPCSTR lpFileName,
		LPDWORD lpFileSizeHigh)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetCompressedFileSizeA(
				lpFileName,
				lpFileSizeHigh);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetCompressedFileSizeA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetCompressedFileSizeW(
		LPCWSTR lpFileName,
		LPDWORD lpFileSizeHigh)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetCompressedFileSizeW(
				lpFileName,
				lpFileSizeHigh);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetCompressedFileSizeW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetDiskFreeSpaceA(
		LPCSTR lpRootPathName,
		LPDWORD lpSectorsPerCluster,
		LPDWORD lpBytesPerSector,
		LPDWORD lpNumberOfFreeClusters,
		LPDWORD lpTotalNumberOfClusters)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetDiskFreeSpaceA(
				lpRootPathName,
				lpSectorsPerCluster,
				lpBytesPerSector,
				lpNumberOfFreeClusters,
				lpTotalNumberOfClusters);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDiskFreeSpaceA));
			message.AppendValue(lpRootPathName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetDiskFreeSpaceW(
		LPCWSTR lpRootPathName,
		LPDWORD lpSectorsPerCluster,
		LPDWORD lpBytesPerSector,
		LPDWORD lpNumberOfFreeClusters,
		LPDWORD lpTotalNumberOfClusters)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetDiskFreeSpaceW(
				lpRootPathName,
				lpSectorsPerCluster,
				lpBytesPerSector,
				lpNumberOfFreeClusters,
				lpTotalNumberOfClusters);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDiskFreeSpaceW));
			message.AppendValue(lpRootPathName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetDiskFreeSpaceExA(
		LPCSTR lpDirectoryName,
		PULARGE_INTEGER lpFreeBytesAvailableToCaller,
		PULARGE_INTEGER lpTotalNumberOfBytes,
		PULARGE_INTEGER lpTotalNumberOfFreeBytes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetDiskFreeSpaceExA(
				lpDirectoryName,
				lpFreeBytesAvailableToCaller,
				lpTotalNumberOfBytes,
				lpTotalNumberOfFreeBytes);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDiskFreeSpaceExA));
			message.AppendValue(lpDirectoryName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetDiskFreeSpaceExW(
		LPCWSTR lpDirectoryName,
		PULARGE_INTEGER lpFreeBytesAvailableToCaller,
		PULARGE_INTEGER lpTotalNumberOfBytes,
		PULARGE_INTEGER lpTotalNumberOfFreeBytes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetDiskFreeSpaceExW(
				lpDirectoryName,
				lpFreeBytesAvailableToCaller,
				lpTotalNumberOfBytes,
				lpTotalNumberOfFreeBytes);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDiskFreeSpaceExW));
			message.AppendValue(lpDirectoryName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	UINT WINAPI GetDriveTypeA(
		LPCSTR lpRootPathName)
	{
		UINT result = 0;
		__try
		{
			result = Cache::FileApi::GetDriveTypeA(
				lpRootPathName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDriveTypeA));
			message.AppendValue(lpRootPathName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	UINT WINAPI GetDriveTypeW(
		LPCWSTR lpRootPathName)
	{
		UINT result = 0;
		__try
		{
			result = Cache::FileApi::GetDriveTypeW(
				lpRootPathName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDriveTypeW));
			message.AppendValue(lpRootPathName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileAttributesA(
		LPCSTR lpFileName)
	{
		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		DWORD result = 0;
		__try
		{
			if (blockAccess)
			{
				result = INVALID_FILE_ATTRIBUTES;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::FileApi::GetFileAttributesA(
					lpFileName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileAttributesW(
		LPCWSTR lpFileName)
	{
		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		DWORD result = 0;
		__try
		{
			if (blockAccess)
			{
				result = INVALID_FILE_ATTRIBUTES;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::FileApi::GetFileAttributesW(
					lpFileName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesExA(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
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
				result = Cache::FileApi::GetFileAttributesExA(
					lpFileName,
					fInfoLevelId,
					lpFileInformation);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesExA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesExW(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
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
				result = Cache::FileApi::GetFileAttributesExW(
					lpFileName,
					fInfoLevelId,
					lpFileInformation);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesExW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileInformationByHandle(
		HANDLE hFile,
		LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetFileInformationByHandle(
				hFile,
				lpFileInformation);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileInformationByHandle));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileSize(
		HANDLE hFile,
		LPDWORD lpFileSizeHigh)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetFileSize(
				hFile,
				lpFileSizeHigh);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileSize));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileSizeEx(
		HANDLE hFile,
		PLARGE_INTEGER lpFileSize)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetFileSizeEx(
				hFile,
				lpFileSize);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileSizeEx));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileTime(
		HANDLE hFile,
		LPFILETIME lpCreationTime,
		LPFILETIME lpLastAccessTime,
		LPFILETIME lpLastWriteTime)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetFileTime(
				hFile,
				lpCreationTime,
				lpLastAccessTime,
				lpLastWriteTime);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileTime));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileType(
		HANDLE hFile)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetFileType(
				hFile);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileType));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFinalPathNameByHandleA(
		HANDLE hFile,
		LPSTR lpszFilePath,
		DWORD cchFilePath,
		DWORD dwFlags)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetFinalPathNameByHandleA(
				hFile,
				lpszFilePath,
				cchFilePath,
				dwFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFinalPathNameByHandleA));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFinalPathNameByHandleW(
		HANDLE hFile,
		LPWSTR lpszFilePath,
		DWORD cchFilePath,
		DWORD dwFlags)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetFinalPathNameByHandleW(
				hFile,
				lpszFilePath,
				cchFilePath,
				dwFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFinalPathNameByHandleW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFullPathNameA(
		LPCSTR lpFileName,
		DWORD nBufferLength,
		LPSTR lpBuffer,
		LPSTR *lpFilePart)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetFullPathNameA(
				lpFileName,
				nBufferLength,
				lpBuffer,
				lpFilePart);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFullPathNameA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFullPathNameW(
		LPCWSTR lpFileName,
		DWORD nBufferLength,
		LPWSTR lpBuffer,
		LPWSTR *lpFilePart)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetFullPathNameW(
				lpFileName,
				nBufferLength,
				lpBuffer,
				lpFilePart);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFullPathNameW));
			message.AppendValue(lpFileName);
			message.AppendValue(lpBuffer);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetLogicalDrives()
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetLogicalDrives();
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLogicalDrives));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetLogicalDriveStringsW(
		DWORD nBufferLength,
		LPWSTR lpBuffer)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetLogicalDriveStringsW(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLogicalDriveStringsW));
			message.AppendValue(lpBuffer);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetLongPathNameA(
		LPCSTR lpszShortPath,
		LPSTR lpszLongPath,
		DWORD cchBuffer)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetLongPathNameA(
				lpszShortPath,
				lpszLongPath,
				cchBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLongPathNameA));
			message.AppendValue(lpszShortPath);
			message.AppendValue(lpszLongPath);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetLongPathNameW(
		LPCWSTR lpszShortPath,
		LPWSTR lpszLongPath,
		DWORD cchBuffer)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetLongPathNameW(
				lpszShortPath,
				lpszLongPath,
				cchBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLongPathNameW));
			message.AppendValue(lpszShortPath);
			message.AppendValue(lpszLongPath);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetShortPathNameW(
		LPCWSTR lpszLongPath,
		LPWSTR lpszShortPath,
		DWORD cchBuffer)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetShortPathNameW(
				lpszLongPath,
				lpszShortPath,
				cchBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetShortPathNameW));
			message.AppendValue(lpszLongPath);
			message.AppendValue(lpszShortPath);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	UINT WINAPI GetTempFileNameA(
		LPCSTR lpPathName,
		LPCSTR lpPrefixString,
		UINT uUnique,
		LPSTR lpTempFileName)
	{
		UINT result = 0;
		__try
		{
			result = Cache::FileApi::GetTempFileNameA(
				lpPathName,
				lpPrefixString,
				uUnique,
				lpTempFileName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetTempFileNameA));
			message.AppendValue(lpPathName);
			message.AppendValue(lpPrefixString);
			message.AppendValue(uUnique);
			message.AppendValue(lpTempFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	UINT WINAPI GetTempFileNameW(
		LPCWSTR lpPathName,
		LPCWSTR lpPrefixString,
		UINT uUnique,
		LPWSTR lpTempFileName)
	{
		UINT result = 0;
		__try
		{
			result = Cache::FileApi::GetTempFileNameW(
				lpPathName,
				lpPrefixString,
				uUnique,
				lpTempFileName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetTempFileNameW));
			message.AppendValue(lpPathName);
			message.AppendValue(lpPrefixString);
			message.AppendValue(uUnique);
			message.AppendValue(lpTempFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetTempPathA(
		DWORD nBufferLength,
		LPSTR lpBuffer)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetTempPathA(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetTempPathA));
			message.AppendValue(lpBuffer);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetTempPathW(
		DWORD nBufferLength,
		LPWSTR lpBuffer)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::GetTempPathW(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetTempPathW));
			message.AppendValue(lpBuffer);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetVolumeInformationA(
		LPCSTR lpRootPathName,
		LPSTR lpVolumeNameBuffer,
		DWORD nVolumeNameSize,
		LPDWORD lpVolumeSerialNumber,
		LPDWORD lpMaximumComponentLength,
		LPDWORD lpFileSystemFlags,
		LPSTR lpFileSystemNameBuffer,
		DWORD nFileSystemNameSize)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetVolumeInformationA(
				lpRootPathName,
				lpVolumeNameBuffer,
				nVolumeNameSize,
				lpVolumeSerialNumber,
				lpMaximumComponentLength,
				lpFileSystemFlags,
				lpFileSystemNameBuffer,
				nFileSystemNameSize);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumeInformationA));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetVolumeInformationW(
		LPCWSTR lpRootPathName,
		LPWSTR lpVolumeNameBuffer,
		DWORD nVolumeNameSize,
		LPDWORD lpVolumeSerialNumber,
		LPDWORD lpMaximumComponentLength,
		LPDWORD lpFileSystemFlags,
		LPWSTR lpFileSystemNameBuffer,
		DWORD nFileSystemNameSize)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetVolumeInformationW(
				lpRootPathName,
				lpVolumeNameBuffer,
				nVolumeNameSize,
				lpVolumeSerialNumber,
				lpMaximumComponentLength,
				lpFileSystemFlags,
				lpFileSystemNameBuffer,
				nFileSystemNameSize);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumeInformationW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetVolumeInformationByHandleW(
		HANDLE hFile,
		LPWSTR lpVolumeNameBuffer,
		DWORD nVolumeNameSize,
		LPDWORD lpVolumeSerialNumber,
		LPDWORD lpMaximumComponentLength,
		LPDWORD lpFileSystemFlags,
		LPWSTR lpFileSystemNameBuffer,
		DWORD nFileSystemNameSize)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetVolumeInformationByHandleW(
				hFile,
				lpVolumeNameBuffer,
				nVolumeNameSize,
				lpVolumeSerialNumber,
				lpMaximumComponentLength,
				lpFileSystemFlags,
				lpFileSystemNameBuffer,
				nFileSystemNameSize);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumeInformationByHandleW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetVolumeNameForVolumeMountPointW(
		LPCWSTR lpszVolumeMountPoint,
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetVolumeNameForVolumeMountPointW(
				lpszVolumeMountPoint,
				lpszVolumeName,
				cchBufferLength);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumeNameForVolumeMountPointW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetVolumePathNamesForVolumeNameW(
		LPCWSTR lpszVolumeName,
		LPWCH lpszVolumePathNames,
		DWORD cchBufferLength,
		PDWORD lpcchReturnLength)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetVolumePathNamesForVolumeNameW(
				lpszVolumeName,
				lpszVolumePathNames,
				cchBufferLength,
				lpcchReturnLength);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumePathNamesForVolumeNameW));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetVolumePathNameW(
		LPCWSTR lpszFileName,
		LPWSTR lpszVolumePathName,
		DWORD cchBufferLength)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::GetVolumePathNameW(
				lpszFileName,
				lpszVolumePathName,
				cchBufferLength);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumePathNameW));
			message.AppendValue(lpszFileName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI LocalFileTimeToFileTime(
		const FILETIME *lpLocalFileTime,
		LPFILETIME lpFileTime)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::LocalFileTimeToFileTime(
				lpLocalFileTime,
				lpFileTime);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LocalFileTimeToFileTime));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI LockFile(
		HANDLE hFile,
		DWORD dwFileOffsetLow,
		DWORD dwFileOffsetHigh,
		DWORD nNumberOfBytesToLockLow,
		DWORD nNumberOfBytesToLockHigh)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::LockFile(
				hFile,
				dwFileOffsetLow,
				dwFileOffsetHigh,
				nNumberOfBytesToLockLow,
				nNumberOfBytesToLockHigh);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LockFile));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI LockFileEx(
		HANDLE hFile,
		DWORD dwFlags,
		DWORD dwReserved,
		DWORD nNumberOfBytesToLockLow,
		DWORD nNumberOfBytesToLockHigh,
		LPOVERLAPPED lpOverlapped)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::LockFileEx(
				hFile,
				dwFlags,
				dwReserved,
				nNumberOfBytesToLockLow,
				nNumberOfBytesToLockHigh,
				lpOverlapped);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LockFileEx));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI QueryDosDeviceW(
		LPCWSTR lpDeviceName,
		LPWSTR lpTargetPath,
		DWORD ucchMax)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::QueryDosDeviceW(
				lpDeviceName,
				lpTargetPath,
				ucchMax);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::QueryDosDeviceW));
			message.AppendValue(lpDeviceName);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI ReadFile(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::ReadFile(
				hFile,
				lpBuffer,
				nNumberOfBytesToRead,
				lpNumberOfBytesRead,
				lpOverlapped);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::ReadFile));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI ReadFileEx(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::ReadFileEx(
				hFile,
				lpBuffer,
				nNumberOfBytesToRead,
				lpOverlapped,
				lpCompletionRoutine);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::ReadFileEx));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI ReadFileScatter(
		HANDLE hFile,
		FILE_SEGMENT_ELEMENT aSegmentArray[],
		DWORD nNumberOfBytesToRead,
		LPDWORD lpReserved,
		LPOVERLAPPED lpOverlapped)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::ReadFileScatter(
				hFile,
				aSegmentArray,
				nNumberOfBytesToRead,
				lpReserved,
				lpOverlapped);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::ReadFileScatter));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryA(
		LPCSTR lpPathName)
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
				result = Cache::FileApi::RemoveDirectoryA(
					lpPathName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::RemoveDirectoryA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryW(
		LPCWSTR lpPathName)
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
				result = Cache::FileApi::RemoveDirectoryW(
					lpPathName);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::RemoveDirectoryW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetEndOfFile(
		HANDLE hFile)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::SetEndOfFile(
				hFile);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetEndOfFile));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	void WINAPI SetFileApisToANSI()
	{
		__try
		{
			Cache::FileApi::SetFileApisToANSI();
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileApisToANSI));
			connectionManager.WriteMessage(message);
		}
	}

	void WINAPI SetFileApisToOEM()
	{
		__try
		{
			Cache::FileApi::SetFileApisToOEM();
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileApisToOEM));
			connectionManager.WriteMessage(message);
		}
	}

	BOOL WINAPI SetFileAttributesA(
		LPCSTR lpFileName,
		DWORD  dwFileAttributes)
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
				result = Cache::FileApi::SetFileAttributesA(
					lpFileName,
					dwFileAttributes);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileAttributesA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesW(
		LPCWSTR lpFileName,
		DWORD  dwFileAttributes)
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
				result = Cache::FileApi::SetFileAttributesW(
					lpFileName,
					dwFileAttributes);
			}
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileAttributesW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileInformationByHandle(
		HANDLE hFile,
		FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
		LPVOID lpFileInformation,
		DWORD dwBufferSize)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::SetFileInformationByHandle(
				hFile,
				FileInformationClass,
				lpFileInformation,
				dwBufferSize);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileInformationByHandle));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileIoOverlappedRange(
		HANDLE FileHandle,
		PUCHAR OverlappedRangeStart,
		ULONG Length)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::SetFileIoOverlappedRange(
				FileHandle,
				OverlappedRangeStart,
				Length);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileIoOverlappedRange));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI SetFilePointer(
		HANDLE hFile,
		LONG lDistanceToMove,
		PLONG lpDistanceToMoveHigh,
		DWORD dwMoveMethod)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::FileApi::SetFilePointer(
				hFile,
				lDistanceToMove,
				lpDistanceToMoveHigh,
				dwMoveMethod);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFilePointer));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFilePointerEx(
		HANDLE hFile,
		LARGE_INTEGER liDistanceToMove,
		PLARGE_INTEGER lpNewFilePointer,
		DWORD dwMoveMethod)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::SetFilePointerEx(
				hFile,
				liDistanceToMove,
				lpNewFilePointer,
				dwMoveMethod);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFilePointerEx));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileTime(
		HANDLE hFile,
		const FILETIME *lpCreationTime,
		const FILETIME *lpLastAccessTime,
		const FILETIME *lpLastWriteTime)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::SetFileTime(
				hFile,
				lpCreationTime,
				lpLastAccessTime,
				lpLastWriteTime);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileTime));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileValidData(
		HANDLE   hFile,
		LONGLONG ValidDataLength)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::SetFileValidData(
				hFile,
				ValidDataLength);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileValidData));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI UnlockFile(
		HANDLE hFile,
		DWORD dwFileOffsetLow,
		DWORD dwFileOffsetHigh,
		DWORD nNumberOfBytesToUnlockLow,
		DWORD nNumberOfBytesToUnlockHigh)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::UnlockFile(
				hFile,
				dwFileOffsetLow,
				dwFileOffsetHigh,
				nNumberOfBytesToUnlockLow,
				nNumberOfBytesToUnlockHigh);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::UnlockFile));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI UnlockFileEx(
		HANDLE hFile,
		DWORD dwReserved,
		DWORD nNumberOfBytesToUnlockLow,
		DWORD nNumberOfBytesToUnlockHigh,
		LPOVERLAPPED lpOverlapped)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::UnlockFileEx(
				hFile,
				dwReserved,
				nNumberOfBytesToUnlockLow,
				nNumberOfBytesToUnlockHigh,
				lpOverlapped);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::UnlockFileEx));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI WriteFile(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten,
		LPOVERLAPPED lpOverlapped)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::WriteFile(
				hFile,
				lpBuffer,
				nNumberOfBytesToWrite,
				lpNumberOfBytesWritten,
				lpOverlapped);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::WriteFile));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI WriteFileEx(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::WriteFileEx(
				hFile,
				lpBuffer,
				nNumberOfBytesToWrite,
				lpOverlapped,
				lpCompletionRoutine);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::WriteFileEx));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI WriteFileGather(
		HANDLE hFile,
		FILE_SEGMENT_ELEMENT aSegmentArray[],
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpReserved,
		LPOVERLAPPED lpOverlapped)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FileApi::WriteFileGather(
				hFile,
				aSegmentArray,
				nNumberOfBytesToWrite,
				lpReserved,
				lpOverlapped);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::WriteFileGather));
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}
}