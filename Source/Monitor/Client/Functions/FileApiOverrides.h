#pragma once
#include "FileApiCache.h"
#include "ConnectionManager.h"
#include "FileSystemAccessSandbox.h"

namespace Functions::FileApi::Overrides
{
	BOOL WINAPI AreFileApisANSI()
	{
		BOOL result = 0;
		__try
		{
			result = Cache::AreFileApisANSI();
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::AreFileApisANSI));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::CompareFileTime(
				lpFileTime1,
				lpFileTime2
			);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CompareFileTime));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryA(
		LPCSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
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
				result = Cache::CreateDirectoryA(
					lpPathName,
					lpSecurityAttributes);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateDirectoryA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryW(
		LPCWSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
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
				result = Cache::CreateDirectoryW(
					lpPathName,
					lpSecurityAttributes);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateDirectoryW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
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
				result = Cache::CreateFile2(
					lpFileName,
					dwDesiredAccess,
					dwShareMode,
					dwCreationDisposition,
					pCreateExParams);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateFile2));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(dwCreationDisposition);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
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
				result = Cache::CreateFileA(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateFileA));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(dwCreationDisposition);
			message.AppendValue(dwFlagsAndAttributes);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
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
				result = Cache::CreateFileW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::CreateFileW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwDesiredAccess);
			message.AppendValue(dwShareMode);
			message.AppendValue(dwCreationDisposition);
			message.AppendValue(dwFlagsAndAttributes);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::DefineDosDeviceW(
				dwFlags,
				lpDeviceName,
				lpTargetPath);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DefineDosDeviceW));
			message.AppendValue(dwFlags);
			message.AppendValue(lpDeviceName);
			message.AppendValue(lpTargetPath);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileA(
		LPCSTR lpFileName)
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
				result = Cache::DeleteFileA(
					lpFileName);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DeleteFileA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileW(
		LPCWSTR lpFileName)
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
				result = Cache::DeleteFileW(
					lpFileName);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DeleteFileW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteVolumeMountPointW(
		LPCWSTR lpszVolumeMountPoint)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::DeleteVolumeMountPointW(
				lpszVolumeMountPoint);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::DeleteVolumeMountPointW));
			message.AppendValue(lpszVolumeMountPoint);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FileTimeToLocalFileTime(
				lpFileTime,
				lpLocalFileTime
			);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FileTimeToLocalFileTime));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindClose(
		HANDLE hFindFile)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FindClose(
				hFindFile);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindClose));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindCloseChangeNotification(
		HANDLE hChangeHandle)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FindCloseChangeNotification(
				hChangeHandle);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindCloseChangeNotification));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstChangeNotificationA(
				lpPathName,
				bWatchSubtree,
				dwNotifyFilter);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstChangeNotificationA));
			message.AppendValue(lpPathName);
			message.AppendValue(bWatchSubtree);
			message.AppendValue(dwNotifyFilter);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstChangeNotificationW(
				lpPathName,
				bWatchSubtree,
				dwNotifyFilter);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstChangeNotificationW));
			message.AppendValue(lpPathName);
			message.AppendValue(bWatchSubtree);
			message.AppendValue(dwNotifyFilter);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstFileA(
				lpFileName,
				lpFindFileData);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstFileA));
			message.AppendValue(lpFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstFileW(
				lpFileName,
				lpFindFileData);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstFileW));
			message.AppendValue(lpFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstFileExA(
				lpFileName,
				fInfoLevelId,
				lpFindFileData,
				fSearchOp,
				lpSearchFilter,
				dwAdditionalFlags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstFileExA));
			message.AppendValue(lpFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstFileExW(
				lpFileName,
				fInfoLevelId,
				lpFindFileData,
				fSearchOp,
				lpSearchFilter,
				dwAdditionalFlags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstFileExW));
			message.AppendValue(lpFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstFileNameW(
				lpFileName,
				dwFlags,
				StringLength,
				LinkName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstFileNameW));
			message.AppendValue(lpFileName);
			message.AppendValue(dwFlags);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstStreamW(
				lpFileName,
				InfoLevel,
				lpFindStreamData,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstStreamW));
			message.AppendValue(lpFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindFirstVolumeW(
				lpszVolumeName,
				cchBufferLength);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindFirstVolumeW));
			message.AppendValue(lpszVolumeName);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindNextChangeNotification(
		HANDLE hChangeHandle)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FindNextChangeNotification(
				hChangeHandle);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindNextChangeNotification));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindNextFileA(
				hFindFile,
				lpFindFileData);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindNextFileA));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindNextFileW(
				hFindFile,
				lpFindFileData);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindNextFileW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindNextFileNameW(
				hFindStream,
				StringLength,
				LinkName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindNextFileNameW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindNextStreamW(
				hFindStream,
				lpFindStreamData);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindNextStreamW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::FindNextVolumeW(
				hFindVolume,
				lpszVolumeName,
				cchBufferLength);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindNextVolumeW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FindVolumeClose(
		HANDLE hFindVolume)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FindVolumeClose(
				hFindVolume);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FindVolumeClose));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI FlushFileBuffers(
		HANDLE hFile)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::FlushFileBuffers(
				hFile);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::FlushFileBuffers));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetCompressedFileSizeA(
				lpFileName,
				lpFileSizeHigh);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetCompressedFileSizeA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetCompressedFileSizeW(
				lpFileName,
				lpFileSizeHigh);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetCompressedFileSizeW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetDiskFreeSpaceA(
				lpRootPathName,
				lpSectorsPerCluster,
				lpBytesPerSector,
				lpNumberOfFreeClusters,
				lpTotalNumberOfClusters);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetDiskFreeSpaceA));
			message.AppendValue(lpRootPathName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetDiskFreeSpaceW(
				lpRootPathName,
				lpSectorsPerCluster,
				lpBytesPerSector,
				lpNumberOfFreeClusters,
				lpTotalNumberOfClusters);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetDiskFreeSpaceW));
			message.AppendValue(lpRootPathName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetDiskFreeSpaceExA(
				lpDirectoryName,
				lpFreeBytesAvailableToCaller,
				lpTotalNumberOfBytes,
				lpTotalNumberOfFreeBytes);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetDiskFreeSpaceExA));
			message.AppendValue(lpDirectoryName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetDiskFreeSpaceExW(
				lpDirectoryName,
				lpFreeBytesAvailableToCaller,
				lpTotalNumberOfBytes,
				lpTotalNumberOfFreeBytes);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetDiskFreeSpaceExW));
			message.AppendValue(lpDirectoryName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	UINT WINAPI GetDriveTypeA(
		LPCSTR lpRootPathName)
	{
		UINT result = 0;
		__try
		{
			result = Cache::GetDriveTypeA(
				lpRootPathName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetDriveTypeA));
			message.AppendValue(lpRootPathName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	UINT WINAPI GetDriveTypeW(
		LPCWSTR lpRootPathName)
	{
		UINT result = 0;
		__try
		{
			result = Cache::GetDriveTypeW(
				lpRootPathName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetDriveTypeW));
			message.AppendValue(lpRootPathName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileAttributesA(
		LPCSTR lpFileName)
	{
		// Check if this file is allowed read access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpFileName);
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
				result = Cache::GetFileAttributesA(
					lpFileName);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileAttributesA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileAttributesW(
		LPCWSTR lpFileName)
	{
		// Check if this file is allowed read access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpFileName);
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
				result = Cache::GetFileAttributesW(
					lpFileName);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileAttributesW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesExA(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
	{
		// Check if this file is allowed read access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = INVALID_FILE_ATTRIBUTES;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::GetFileAttributesExA(
					lpFileName,
					fInfoLevelId,
					lpFileInformation);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileAttributesExA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesExW(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
	{
		// Check if this file is allowed read access
		bool blockAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		BOOL result = 0;
		__try
		{
			if (blockAccess)
			{
				result = INVALID_FILE_ATTRIBUTES;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::GetFileAttributesExW(
					lpFileName,
					fInfoLevelId,
					lpFileInformation);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileAttributesExW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFileInformationByHandle(
				hFile,
				lpFileInformation);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileInformationByHandle));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFileSize(
				hFile,
				lpFileSizeHigh);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileSize));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFileSizeEx(
				hFile,
				lpFileSize);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileSizeEx));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFileTime(
				hFile,
				lpCreationTime,
				lpLastAccessTime,
				lpLastWriteTime);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileTime));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileType(
		HANDLE hFile)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::GetFileType(
				hFile);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFileType));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFinalPathNameByHandleA(
				hFile,
				lpszFilePath,
				cchFilePath,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFinalPathNameByHandleA));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFinalPathNameByHandleW(
				hFile,
				lpszFilePath,
				cchFilePath,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFinalPathNameByHandleW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFullPathNameA(
				lpFileName,
				nBufferLength,
				lpBuffer,
				lpFilePart);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFullPathNameA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetFullPathNameW(
				lpFileName,
				nBufferLength,
				lpBuffer,
				lpFilePart);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetFullPathNameW));
			message.AppendValue(lpFileName);
			message.AppendValue(lpBuffer);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetLogicalDrives()
	{
		DWORD result = 0;
		__try
		{
			result = Cache::GetLogicalDrives();
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetLogicalDrives));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetLogicalDriveStringsW(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetLogicalDriveStringsW));
			message.AppendValue(lpBuffer);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetLongPathNameA(
				lpszShortPath,
				lpszLongPath,
				cchBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetLongPathNameA));
			message.AppendValue(lpszShortPath);
			message.AppendValue(lpszLongPath);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetLongPathNameW(
				lpszShortPath,
				lpszLongPath,
				cchBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetLongPathNameW));
			message.AppendValue(lpszShortPath);
			message.AppendValue(lpszLongPath);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetShortPathNameW(
				lpszLongPath,
				lpszShortPath,
				cchBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetShortPathNameW));
			message.AppendValue(lpszLongPath);
			message.AppendValue(lpszShortPath);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetTempFileNameA(
				lpPathName,
				lpPrefixString,
				uUnique,
				lpTempFileName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetTempFileNameA));
			message.AppendValue(lpPathName);
			message.AppendValue(lpPrefixString);
			message.AppendValue(uUnique);
			message.AppendValue(lpTempFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetTempFileNameW(
				lpPathName,
				lpPrefixString,
				uUnique,
				lpTempFileName);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetTempFileNameW));
			message.AppendValue(lpPathName);
			message.AppendValue(lpPrefixString);
			message.AppendValue(uUnique);
			message.AppendValue(lpTempFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetTempPathA(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetTempPathA));
			message.AppendValue(lpBuffer);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetTempPathW(
				nBufferLength,
				lpBuffer);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetTempPathW));
			message.AppendValue(lpBuffer);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetVolumeInformationA(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetVolumeInformationA));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetVolumeInformationW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetVolumeInformationW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetVolumeInformationByHandleW(
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetVolumeInformationByHandleW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetVolumeNameForVolumeMountPointW(
				lpszVolumeMountPoint,
				lpszVolumeName,
				cchBufferLength);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetVolumeNameForVolumeMountPointW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetVolumePathNamesForVolumeNameW(
				lpszVolumeName,
				lpszVolumePathNames,
				cchBufferLength,
				lpcchReturnLength);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetVolumePathNamesForVolumeNameW));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::GetVolumePathNameW(
				lpszFileName,
				lpszVolumePathName,
				cchBufferLength);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::GetVolumePathNameW));
			message.AppendValue(lpszFileName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::LocalFileTimeToFileTime(
				lpLocalFileTime,
				lpFileTime);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LocalFileTimeToFileTime));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::LockFile(
				hFile,
				dwFileOffsetLow,
				dwFileOffsetHigh,
				nNumberOfBytesToLockLow,
				nNumberOfBytesToLockHigh);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LockFile));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::LockFileEx(
				hFile,
				dwFlags,
				dwReserved,
				nNumberOfBytesToLockLow,
				nNumberOfBytesToLockHigh,
				lpOverlapped);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LockFileEx));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::QueryDosDeviceW(
				lpDeviceName,
				lpTargetPath,
				ucchMax);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::QueryDosDeviceW));
			message.AppendValue(lpDeviceName);
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::ReadFile(
				hFile,
				lpBuffer,
				nNumberOfBytesToRead,
				lpNumberOfBytesRead,
				lpOverlapped);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::ReadFile));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::ReadFileEx(
				hFile,
				lpBuffer,
				nNumberOfBytesToRead,
				lpOverlapped,
				lpCompletionRoutine);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::ReadFileEx));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::ReadFileScatter(
				hFile,
				aSegmentArray,
				nNumberOfBytesToRead,
				lpReserved,
				lpOverlapped);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::ReadFileScatter));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryA(
		LPCSTR lpPathName)
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
				result = Cache::RemoveDirectoryA(
					lpPathName);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::RemoveDirectoryA));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryW(
		LPCWSTR lpPathName)
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
				result = Cache::RemoveDirectoryW(
					lpPathName);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::RemoveDirectoryW));
			message.AppendValue(lpPathName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetEndOfFile(
		HANDLE hFile)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetEndOfFile(
				hFile);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetEndOfFile));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	void WINAPI SetFileApisToANSI()
	{
		__try
		{
			Cache::SetFileApisToANSI();
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileApisToANSI));
			Monitor::ConnectionManager::WriteMessage(message);
		}
	}

	void WINAPI SetFileApisToOEM()
	{
		__try
		{
			Cache::SetFileApisToOEM();
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileApisToOEM));
			Monitor::ConnectionManager::WriteMessage(message);
		}
	}

	BOOL WINAPI SetFileAttributesA(
		LPCSTR lpFileName,
		DWORD  dwFileAttributes)
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
				result = Cache::SetFileAttributesA(
					lpFileName,
					dwFileAttributes);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileAttributesA));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesW(
		LPCWSTR lpFileName,
		DWORD  dwFileAttributes)
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
				result = Cache::SetFileAttributesW(
					lpFileName,
					dwFileAttributes);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileAttributesW));
			message.AppendValue(lpFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileInformationByHandle(
				hFile,
				FileInformationClass,
				lpFileInformation,
				dwBufferSize);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileInformationByHandle));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileIoOverlappedRange(
				FileHandle,
				OverlappedRangeStart,
				Length);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileIoOverlappedRange));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFilePointer(
				hFile,
				lDistanceToMove,
				lpDistanceToMoveHigh,
				dwMoveMethod);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFilePointer));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFilePointerEx(
				hFile,
				liDistanceToMove,
				lpNewFilePointer,
				dwMoveMethod);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFilePointerEx));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileTime(
				hFile,
				lpCreationTime,
				lpLastAccessTime,
				lpLastWriteTime);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileTime));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::SetFileValidData(
				hFile,
				ValidDataLength);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SetFileValidData));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::UnlockFile(
				hFile,
				dwFileOffsetLow,
				dwFileOffsetHigh,
				nNumberOfBytesToUnlockLow,
				nNumberOfBytesToUnlockHigh);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::UnlockFile));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::UnlockFileEx(
				hFile,
				dwReserved,
				nNumberOfBytesToUnlockLow,
				nNumberOfBytesToUnlockHigh,
				lpOverlapped);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::UnlockFileEx));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::WriteFile(
				hFile,
				lpBuffer,
				nNumberOfBytesToWrite,
				lpNumberOfBytesWritten,
				lpOverlapped);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::WriteFile));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::WriteFileEx(
				hFile,
				lpBuffer,
				nNumberOfBytesToWrite,
				lpOverlapped,
				lpCompletionRoutine);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::WriteFileEx));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
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
			result = Cache::WriteFileGather(
				hFile,
				aSegmentArray,
				nNumberOfBytesToWrite,
				lpReserved,
				lpOverlapped);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::WriteFileGather));
			message.AppendValue(result);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}
}