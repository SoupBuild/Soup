#pragma once
#include "FileApiCache.h"
#include "EventLogger.h"

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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::AreFileApisANSI;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CompareFileTime;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryA(
		LPCSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CreateDirectoryA(
				lpPathName,
				lpSecurityAttributes);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateDirectoryA;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateDirectoryW(
		LPCWSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CreateDirectoryW(
				lpPathName,
				lpSecurityAttributes);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateDirectoryW;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		HANDLE result = 0;
		__try
		{
			result = Cache::CreateFile2(
				lpFileName,
				dwDesiredAccess,
				dwShareMode,
				dwCreationDisposition,
				pCreateExParams);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateFile2;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwDesiredAccess);
			EventLogger::AppendValue(message, dwShareMode);
			EventLogger::AppendValue(message, dwCreationDisposition);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		HANDLE result = 0;
		__try
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
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateFileA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwDesiredAccess);
			EventLogger::AppendValue(message, dwShareMode);
			EventLogger::AppendValue(message, dwCreationDisposition);
			EventLogger::AppendValue(message, dwFlagsAndAttributes);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
		HANDLE result = 0;
		__try
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
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateFileW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwDesiredAccess);
			EventLogger::AppendValue(message, dwShareMode);
			EventLogger::AppendValue(message, dwCreationDisposition);
			EventLogger::AppendValue(message, dwFlagsAndAttributes);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DefineDosDeviceW;
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::AppendValue(message, lpDeviceName);
			EventLogger::AppendValue(message, lpTargetPath);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileA(
		LPCSTR lpFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::DeleteFileA(
				lpFileName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DeleteFileA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI DeleteFileW(
		LPCWSTR lpFileName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::DeleteFileW(
				lpFileName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DeleteFileW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::DeleteVolumeMountPointW;
			EventLogger::AppendValue(message, lpszVolumeMountPoint);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FileTimeToLocalFileTime;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindClose;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindCloseChangeNotification;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstChangeNotificationA;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, bWatchSubtree);
			EventLogger::AppendValue(message, dwNotifyFilter);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstChangeNotificationW;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, bWatchSubtree);
			EventLogger::AppendValue(message, dwNotifyFilter);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstFileA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstFileW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstFileExA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstFileExW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstFileNameW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, dwFlags);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstStreamW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindFirstVolumeW;
			EventLogger::AppendValue(message, lpszVolumeName);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindNextChangeNotification;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindNextFileA;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindNextFileW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindNextFileNameW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindNextStreamW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindNextVolumeW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FindVolumeClose;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::FlushFileBuffers;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetCompressedFileSizeA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetCompressedFileSizeW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetDiskFreeSpaceA;
			EventLogger::AppendValue(message, lpRootPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetDiskFreeSpaceW;
			EventLogger::AppendValue(message, lpRootPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetDiskFreeSpaceExA;
			EventLogger::AppendValue(message, lpDirectoryName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetDiskFreeSpaceExW;
			EventLogger::AppendValue(message, lpDirectoryName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetDriveTypeA;
			EventLogger::AppendValue(message, lpRootPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetDriveTypeW;
			EventLogger::AppendValue(message, lpRootPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileAttributesA(
		LPCSTR lpFileName)
	{
		bool result = 0;
		__try
		{
			result = Cache::GetFileAttributesA(
				lpFileName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileAttributesA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI GetFileAttributesW(
		LPCWSTR lpFileName)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::GetFileAttributesW(
				lpFileName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileAttributesW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesExA(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::GetFileAttributesExA(
				lpFileName,
				fInfoLevelId,
				lpFileInformation);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileAttributesExA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI GetFileAttributesExW(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::GetFileAttributesExW(
				lpFileName,
				fInfoLevelId,
				lpFileInformation);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileAttributesExW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileInformationByHandle;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileSize;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileSizeEx;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileTime;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFileType;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFinalPathNameByHandleA;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFinalPathNameByHandleW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFullPathNameA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetFullPathNameW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, lpBuffer);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetLogicalDrives;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetLogicalDriveStringsW;
			EventLogger::AppendValue(message, lpBuffer);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetLongPathNameA;
			EventLogger::AppendValue(message, lpszShortPath);
			EventLogger::AppendValue(message, lpszLongPath);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetLongPathNameW;
			EventLogger::AppendValue(message, lpszShortPath);
			EventLogger::AppendValue(message, lpszLongPath);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetShortPathNameW;
			EventLogger::AppendValue(message, lpszLongPath);
			EventLogger::AppendValue(message, lpszShortPath);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetTempFileNameA;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, lpPrefixString);
			EventLogger::AppendValue(message, uUnique);
			EventLogger::AppendValue(message, lpTempFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetTempFileNameW;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, lpPrefixString);
			EventLogger::AppendValue(message, uUnique);
			EventLogger::AppendValue(message, lpTempFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetTempPathA;
			EventLogger::AppendValue(message, lpBuffer);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetTempPathW;
			EventLogger::AppendValue(message, lpBuffer);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetVolumeInformationA;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetVolumeInformationW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetVolumeInformationByHandleW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetVolumeNameForVolumeMountPointW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetVolumePathNamesForVolumeNameW;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::GetVolumePathNameW;
			EventLogger::AppendValue(message, lpszFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LocalFileTimeToFileTime;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LockFile;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LockFileEx;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::QueryDosDeviceW;
			EventLogger::AppendValue(message, lpDeviceName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::ReadFile;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::ReadFileEx;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::ReadFileScatter;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryA(
		LPCSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::RemoveDirectoryA(
				lpPathName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::RemoveDirectoryA;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI RemoveDirectoryW(
		LPCWSTR lpPathName)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::RemoveDirectoryW(
				lpPathName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::RemoveDirectoryW;
			EventLogger::AppendValue(message, lpPathName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetEndOfFile;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileApisToANSI;
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileApisToOEM;
			EventLogger::WriteMessage(message);
		}
	}

	BOOL WINAPI SetFileAttributesA(
		LPCSTR lpFileName,
		DWORD  dwFileAttributes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetFileAttributesA(
				lpFileName,
				dwFileAttributes);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileAttributesA;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI SetFileAttributesW(
		LPCWSTR lpFileName,
		DWORD  dwFileAttributes)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::SetFileAttributesW(
				lpFileName,
				dwFileAttributes);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileAttributesW;
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileInformationByHandle;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileIoOverlappedRange;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFilePointer;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFilePointerEx;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileTime;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SetFileValidData;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::UnlockFile;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::UnlockFileEx;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::WriteFile;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::WriteFileEx;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::WriteFileGather;
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}
}