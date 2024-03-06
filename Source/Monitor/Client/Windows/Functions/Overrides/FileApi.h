#pragma once
#include "../Cache/FileApi.h"
#include "Windows/ConnectionManager.h"
#include "FileSystemAccessSandbox.h"

namespace Monitor::Windows::Functions::Overrides::FileApi
{
	BOOL WINAPI AreFileApisANSI()
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::AreFileApisANSI));

		BOOL result = Cache::FileApi::AreFileApisANSI();

		message.AppendValue(result);

		return result;
	}

	LONG WINAPI CompareFileTime(
		const FILETIME *lpFileTime1,
		const FILETIME *lpFileTime2)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CompareFileTime));

		LONG result = Cache::FileApi::CompareFileTime(lpFileTime1, lpFileTime2);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI CreateDirectoryA(
		LPCSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryA));

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
			result = Cache::FileApi::CreateDirectoryA(
				lpPathName,
				lpSecurityAttributes);
		}

		message.AppendValue(lpPathName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI CreateDirectoryW(
		LPCWSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateDirectoryW));

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
			result = Cache::FileApi::CreateDirectoryW(
				lpPathName,
				lpSecurityAttributes);
		}

		message.AppendValue(lpPathName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	HANDLE WINAPI CreateFile2(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFile2));

		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;

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

		message.AppendValue(lpFileName);
		message.AppendValue(dwDesiredAccess);
		message.AppendValue(dwShareMode);
		message.AppendValue(dwCreationDisposition);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFileA));

		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;

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
		
		message.AppendValue(lpFileName);
		message.AppendValue(dwDesiredAccess);
		message.AppendValue(dwShareMode);
		message.AppendValue(dwCreationDisposition);
		message.AppendValue(dwFlagsAndAttributes);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::CreateFileW));

		// Check if this file is allowed access
		bool blockAccess = !FileSystemAccessSandbox::IsAllowed(lpFileName, dwDesiredAccess);
		HANDLE result = 0;

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

		message.AppendValue(lpFileName);
		message.AppendValue(dwDesiredAccess);
		message.AppendValue(dwShareMode);
		message.AppendValue(dwCreationDisposition);
		message.AppendValue(dwFlagsAndAttributes);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI DefineDosDeviceW(
		DWORD dwFlags,
		LPCWSTR lpDeviceName,
		LPCWSTR lpTargetPath)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DefineDosDeviceW));

		BOOL result = Cache::FileApi::DefineDosDeviceW(
				dwFlags,
				lpDeviceName,
				lpTargetPath);

		message.AppendValue(dwFlags);
		message.AppendValue(lpDeviceName);
		message.AppendValue(lpTargetPath);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI DeleteFileA(
		LPCSTR lpFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFileA));

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
			result = Cache::FileApi::DeleteFileA(lpFileName);
		}

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI DeleteFileW(
		LPCWSTR lpFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteFileW));

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
			result = Cache::FileApi::DeleteFileW(lpFileName);
		}

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI DeleteVolumeMountPointW(
		LPCWSTR lpszVolumeMountPoint)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::DeleteVolumeMountPointW));

		BOOL result = Cache::FileApi::DeleteVolumeMountPointW(lpszVolumeMountPoint);

		message.AppendValue(lpszVolumeMountPoint);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FileTimeToLocalFileTime(
		const FILETIME *lpFileTime,
		LPFILETIME lpLocalFileTime)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FileTimeToLocalFileTime));

		BOOL result = Cache::FileApi::FileTimeToLocalFileTime(lpFileTime, lpLocalFileTime);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FindClose(
		HANDLE hFindFile)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindClose));

		BOOL result = Cache::FileApi::FindClose(hFindFile);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FindCloseChangeNotification(
		HANDLE hChangeHandle)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindCloseChangeNotification));

		BOOL result = Cache::FileApi::FindCloseChangeNotification(hChangeHandle);

		message.AppendValue(result);

		return result;
	}

	HANDLE WINAPI FindFirstChangeNotificationA(
		LPCSTR lpPathName,
		BOOL bWatchSubtree,
		DWORD dwNotifyFilter)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstChangeNotificationA));

		HANDLE result = Cache::FileApi::FindFirstChangeNotificationA(
			lpPathName,
			bWatchSubtree,
			dwNotifyFilter);

		message.AppendValue(lpPathName);
		message.AppendValue(bWatchSubtree);
		message.AppendValue(dwNotifyFilter);

		return result;
	}

	HANDLE WINAPI FindFirstChangeNotificationW(
		LPCWSTR lpPathName,
		BOOL bWatchSubtree,
		DWORD dwNotifyFilter)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstChangeNotificationW));

		HANDLE result = Cache::FileApi::FindFirstChangeNotificationW(
			lpPathName,
			bWatchSubtree,
			dwNotifyFilter);

		message.AppendValue(lpPathName);
		message.AppendValue(bWatchSubtree);
		message.AppendValue(dwNotifyFilter);

		return result;
	}

	HANDLE WINAPI FindFirstFileA(
		LPCSTR lpFileName,
		LPWIN32_FIND_DATAA lpFindFileData)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileA));

		HANDLE result = Cache::FileApi::FindFirstFileA(lpFileName, lpFindFileData);

		message.AppendValue(lpFileName);

		return result;
	}

	HANDLE WINAPI FindFirstFileW(
		LPCWSTR lpFileName,
		LPWIN32_FIND_DATAW lpFindFileData)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileW));

		HANDLE result = Cache::FileApi::FindFirstFileW(lpFileName, lpFindFileData);

		message.AppendValue(lpFileName);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileExA));

		HANDLE result = Cache::FileApi::FindFirstFileExA(
			lpFileName,
			fInfoLevelId,
			lpFindFileData,
			fSearchOp,
			lpSearchFilter,
			dwAdditionalFlags);

		message.AppendValue(lpFileName);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileExW));

		HANDLE result = Cache::FileApi::FindFirstFileExW(
			lpFileName,
			fInfoLevelId,
			lpFindFileData,
			fSearchOp,
			lpSearchFilter,
			dwAdditionalFlags);

		message.AppendValue(lpFileName);

		return result;
	}

	HANDLE WINAPI FindFirstFileNameW(
		LPCWSTR lpFileName,
		DWORD dwFlags,
		LPDWORD StringLength,
		PWSTR LinkName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstFileNameW));

		HANDLE result = Cache::FileApi::FindFirstFileNameW(
			lpFileName,
			dwFlags,
			StringLength,
			LinkName);

		message.AppendValue(lpFileName);
		message.AppendValue(dwFlags);

		return result;
	}

	HANDLE WINAPI FindFirstStreamW(
		LPCWSTR lpFileName,
		STREAM_INFO_LEVELS InfoLevel,
		LPVOID lpFindStreamData,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstStreamW));

		HANDLE result = Cache::FileApi::FindFirstStreamW(
			lpFileName,
			InfoLevel,
			lpFindStreamData,
			dwFlags);

		message.AppendValue(lpFileName);

		return result;
	}

	HANDLE WINAPI FindFirstVolumeW(
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindFirstVolumeW));

		HANDLE result = Cache::FileApi::FindFirstVolumeW(
			lpszVolumeName,
			cchBufferLength);

		message.AppendValue(lpszVolumeName);

		return result;
	}

	BOOL WINAPI FindNextChangeNotification(
		HANDLE hChangeHandle)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextChangeNotification));

		BOOL result = Cache::FileApi::FindNextChangeNotification(hChangeHandle);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FindNextFileA(
		HANDLE hFindFile,
		LPWIN32_FIND_DATAA lpFindFileData)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextFileA));

		BOOL result = Cache::FileApi::FindNextFileA(hFindFile, lpFindFileData);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FindNextFileW(
		HANDLE hFindFile,
		LPWIN32_FIND_DATAW lpFindFileData)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextFileW));

		BOOL result = Cache::FileApi::FindNextFileW(hFindFile, lpFindFileData);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FindNextFileNameW(
		HANDLE hFindStream,
		LPDWORD StringLength,
		PWSTR LinkName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextFileNameW));

		BOOL result = Cache::FileApi::FindNextFileNameW(
			hFindStream,
			StringLength,
			LinkName);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FindNextStreamW(
		HANDLE hFindStream,
		LPVOID lpFindStreamData)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextStreamW));

		BOOL result = Cache::FileApi::FindNextStreamW(hFindStream, lpFindStreamData);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FindNextVolumeW(
		HANDLE hFindVolume,
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindNextVolumeW));

		BOOL result = Cache::FileApi::FindNextVolumeW(
			hFindVolume,
			lpszVolumeName,
			cchBufferLength);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FindVolumeClose(
		HANDLE hFindVolume)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FindVolumeClose));

		BOOL result = Cache::FileApi::FindVolumeClose(hFindVolume);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI FlushFileBuffers(
		HANDLE hFile)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::FlushFileBuffers));

		BOOL result = Cache::FileApi::FlushFileBuffers(hFile);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetCompressedFileSizeA(
		LPCSTR lpFileName,
		LPDWORD lpFileSizeHigh)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetCompressedFileSizeA));

		DWORD result = Cache::FileApi::GetCompressedFileSizeA(lpFileName, lpFileSizeHigh);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetCompressedFileSizeW(
		LPCWSTR lpFileName,
		LPDWORD lpFileSizeHigh)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetCompressedFileSizeW));

		DWORD result = Cache::FileApi::GetCompressedFileSizeW(lpFileName, lpFileSizeHigh);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetDiskFreeSpaceA(
		LPCSTR lpRootPathName,
		LPDWORD lpSectorsPerCluster,
		LPDWORD lpBytesPerSector,
		LPDWORD lpNumberOfFreeClusters,
		LPDWORD lpTotalNumberOfClusters)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDiskFreeSpaceA));

		BOOL result = Cache::FileApi::GetDiskFreeSpaceA(
			lpRootPathName,
			lpSectorsPerCluster,
			lpBytesPerSector,
			lpNumberOfFreeClusters,
			lpTotalNumberOfClusters);

		message.AppendValue(lpRootPathName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetDiskFreeSpaceW(
		LPCWSTR lpRootPathName,
		LPDWORD lpSectorsPerCluster,
		LPDWORD lpBytesPerSector,
		LPDWORD lpNumberOfFreeClusters,
		LPDWORD lpTotalNumberOfClusters)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDiskFreeSpaceW));

		BOOL result = Cache::FileApi::GetDiskFreeSpaceW(
			lpRootPathName,
			lpSectorsPerCluster,
			lpBytesPerSector,
			lpNumberOfFreeClusters,
			lpTotalNumberOfClusters);

		message.AppendValue(lpRootPathName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetDiskFreeSpaceExA(
		LPCSTR lpDirectoryName,
		PULARGE_INTEGER lpFreeBytesAvailableToCaller,
		PULARGE_INTEGER lpTotalNumberOfBytes,
		PULARGE_INTEGER lpTotalNumberOfFreeBytes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDiskFreeSpaceExA));

		BOOL result = Cache::FileApi::GetDiskFreeSpaceExA(
			lpDirectoryName,
			lpFreeBytesAvailableToCaller,
			lpTotalNumberOfBytes,
			lpTotalNumberOfFreeBytes);

		message.AppendValue(lpDirectoryName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetDiskFreeSpaceExW(
		LPCWSTR lpDirectoryName,
		PULARGE_INTEGER lpFreeBytesAvailableToCaller,
		PULARGE_INTEGER lpTotalNumberOfBytes,
		PULARGE_INTEGER lpTotalNumberOfFreeBytes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDiskFreeSpaceExW));

		BOOL result = Cache::FileApi::GetDiskFreeSpaceExW(
			lpDirectoryName,
			lpFreeBytesAvailableToCaller,
			lpTotalNumberOfBytes,
			lpTotalNumberOfFreeBytes);

		message.AppendValue(lpDirectoryName);
		message.AppendValue(result);

		return result;
	}

	UINT WINAPI GetDriveTypeA(
		LPCSTR lpRootPathName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDriveTypeA));

		UINT result = Cache::FileApi::GetDriveTypeA(lpRootPathName);

		message.AppendValue(lpRootPathName);
		message.AppendValue(result);

		return result;
	}

	UINT WINAPI GetDriveTypeW(
		LPCWSTR lpRootPathName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetDriveTypeW));

		UINT result = Cache::FileApi::GetDriveTypeW(lpRootPathName);

		message.AppendValue(lpRootPathName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetFileAttributesA(
		LPCSTR lpFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesA));

		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		DWORD result = 0;
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

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	DWORD WINAPI GetFileAttributesW(
		LPCWSTR lpFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesW));

		// Check if this file is allowed read access
		bool blockAccess = !FileSystemAccessSandbox::IsReadAllowed(lpFileName);
		DWORD result = 0;
		if (blockAccess)
		{
			result = INVALID_FILE_ATTRIBUTES;
			SetLastError(ERROR_ACCESS_DENIED);
		}
		else
		{
			result = Cache::FileApi::GetFileAttributesW(lpFileName);
		}

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI GetFileAttributesExA(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesExA));

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
			result = Cache::FileApi::GetFileAttributesExA(
				lpFileName,
				fInfoLevelId,
				lpFileInformation);
		}

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI GetFileAttributesExW(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileAttributesExW));

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
			result = Cache::FileApi::GetFileAttributesExW(
				lpFileName,
				fInfoLevelId,
				lpFileInformation);
		}

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI GetFileInformationByHandle(
		HANDLE hFile,
		LPBY_HANDLE_FILE_INFORMATION lpFileInformation)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileInformationByHandle));

		BOOL result = Cache::FileApi::GetFileInformationByHandle(hFile, lpFileInformation);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetFileSize(
		HANDLE hFile,
		LPDWORD lpFileSizeHigh)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileSize));

		DWORD result = Cache::FileApi::GetFileSize(hFile, lpFileSizeHigh);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetFileSizeEx(
		HANDLE hFile,
		PLARGE_INTEGER lpFileSize)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileSizeEx));

		BOOL result = Cache::FileApi::GetFileSizeEx(hFile, lpFileSize);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetFileTime(
		HANDLE hFile,
		LPFILETIME lpCreationTime,
		LPFILETIME lpLastAccessTime,
		LPFILETIME lpLastWriteTime)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileTime));

		BOOL result = Cache::FileApi::GetFileTime(
			hFile,
			lpCreationTime,
			lpLastAccessTime,
			lpLastWriteTime);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetFileType(
		HANDLE hFile)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFileType));

		DWORD result = Cache::FileApi::GetFileType(hFile);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetFinalPathNameByHandleA(
		HANDLE hFile,
		LPSTR lpszFilePath,
		DWORD cchFilePath,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFinalPathNameByHandleA));

		DWORD result = Cache::FileApi::GetFinalPathNameByHandleA(
			hFile,
			lpszFilePath,
			cchFilePath,
			dwFlags);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetFinalPathNameByHandleW(
		HANDLE hFile,
		LPWSTR lpszFilePath,
		DWORD cchFilePath,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFinalPathNameByHandleW));

		DWORD result = Cache::FileApi::GetFinalPathNameByHandleW(
			hFile,
			lpszFilePath,
			cchFilePath,
			dwFlags);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetFullPathNameA(
		LPCSTR lpFileName,
		DWORD nBufferLength,
		LPSTR lpBuffer,
		LPSTR *lpFilePart)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFullPathNameA));

		DWORD result = Cache::FileApi::GetFullPathNameA(
			lpFileName,
			nBufferLength,
			lpBuffer,
			lpFilePart);

		message.AppendValue(lpFileName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetFullPathNameW(
		LPCWSTR lpFileName,
		DWORD nBufferLength,
		LPWSTR lpBuffer,
		LPWSTR *lpFilePart)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetFullPathNameW));

		DWORD result = Cache::FileApi::GetFullPathNameW(
			lpFileName,
			nBufferLength,
			lpBuffer,
			lpFilePart);

		message.AppendValue(lpFileName);
		message.AppendValue(lpBuffer);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetLogicalDrives()
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLogicalDrives));

		DWORD result = Cache::FileApi::GetLogicalDrives();

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetLogicalDriveStringsW(
		DWORD nBufferLength,
		LPWSTR lpBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLogicalDriveStringsW));

		DWORD result = Cache::FileApi::GetLogicalDriveStringsW(nBufferLength, lpBuffer);

		message.AppendValue(lpBuffer);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetLongPathNameA(
		LPCSTR lpszShortPath,
		LPSTR lpszLongPath,
		DWORD cchBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLongPathNameA));

		DWORD result = Cache::FileApi::GetLongPathNameA(
			lpszShortPath,
			lpszLongPath,
			cchBuffer);

		message.AppendValue(lpszShortPath);
		message.AppendValue(lpszLongPath);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetLongPathNameW(
		LPCWSTR lpszShortPath,
		LPWSTR lpszLongPath,
		DWORD cchBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetLongPathNameW));

		DWORD result = Cache::FileApi::GetLongPathNameW(
			lpszShortPath,
			lpszLongPath,
			cchBuffer);

		message.AppendValue(lpszShortPath);
		message.AppendValue(lpszLongPath);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetShortPathNameW(
		LPCWSTR lpszLongPath,
		LPWSTR lpszShortPath,
		DWORD cchBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetShortPathNameW));

		DWORD result = Cache::FileApi::GetShortPathNameW(
			lpszLongPath,
			lpszShortPath,
			cchBuffer);

		message.AppendValue(lpszLongPath);
		message.AppendValue(lpszShortPath);
		message.AppendValue(result);

		return result;
	}

	UINT WINAPI GetTempFileNameA(
		LPCSTR lpPathName,
		LPCSTR lpPrefixString,
		UINT uUnique,
		LPSTR lpTempFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetTempFileNameA));

		UINT result = Cache::FileApi::GetTempFileNameA(
			lpPathName,
			lpPrefixString,
			uUnique,
			lpTempFileName);

		message.AppendValue(lpPathName);
		message.AppendValue(lpPrefixString);
		message.AppendValue(uUnique);
		message.AppendValue(lpTempFileName);
		message.AppendValue(result);

		return result;
	}

	UINT WINAPI GetTempFileNameW(
		LPCWSTR lpPathName,
		LPCWSTR lpPrefixString,
		UINT uUnique,
		LPWSTR lpTempFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetTempFileNameW));

		UINT result = Cache::FileApi::GetTempFileNameW(
			lpPathName,
			lpPrefixString,
			uUnique,
			lpTempFileName);

		message.AppendValue(lpPathName);
		message.AppendValue(lpPrefixString);
		message.AppendValue(uUnique);
		message.AppendValue(lpTempFileName);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetTempPathA(
		DWORD nBufferLength,
		LPSTR lpBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetTempPathA));

		DWORD result = Cache::FileApi::GetTempPathA(nBufferLength, lpBuffer);

		message.AppendValue(lpBuffer);
		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI GetTempPathW(
		DWORD nBufferLength,
		LPWSTR lpBuffer)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetTempPathW));

		DWORD result = Cache::FileApi::GetTempPathW(nBufferLength, lpBuffer);

		message.AppendValue(lpBuffer);
		message.AppendValue(result);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumeInformationA));

		BOOL result = Cache::FileApi::GetVolumeInformationA(
			lpRootPathName,
			lpVolumeNameBuffer,
			nVolumeNameSize,
			lpVolumeSerialNumber,
			lpMaximumComponentLength,
			lpFileSystemFlags,
			lpFileSystemNameBuffer,
			nFileSystemNameSize);

		message.AppendValue(result);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumeInformationW));

		BOOL result = Cache::FileApi::GetVolumeInformationW(
			lpRootPathName,
			lpVolumeNameBuffer,
			nVolumeNameSize,
			lpVolumeSerialNumber,
			lpMaximumComponentLength,
			lpFileSystemFlags,
			lpFileSystemNameBuffer,
			nFileSystemNameSize);

		message.AppendValue(result);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumeInformationByHandleW));

		BOOL result = Cache::FileApi::GetVolumeInformationByHandleW(
			hFile,
			lpVolumeNameBuffer,
			nVolumeNameSize,
			lpVolumeSerialNumber,
			lpMaximumComponentLength,
			lpFileSystemFlags,
			lpFileSystemNameBuffer,
			nFileSystemNameSize);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetVolumeNameForVolumeMountPointW(
		LPCWSTR lpszVolumeMountPoint,
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumeNameForVolumeMountPointW));

		BOOL result = Cache::FileApi::GetVolumeNameForVolumeMountPointW(
			lpszVolumeMountPoint,
			lpszVolumeName,
			cchBufferLength);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetVolumePathNamesForVolumeNameW(
		LPCWSTR lpszVolumeName,
		LPWCH lpszVolumePathNames,
		DWORD cchBufferLength,
		PDWORD lpcchReturnLength)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumePathNamesForVolumeNameW));

		BOOL result = Cache::FileApi::GetVolumePathNamesForVolumeNameW(
			lpszVolumeName,
			lpszVolumePathNames,
			cchBufferLength,
			lpcchReturnLength);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI GetVolumePathNameW(
		LPCWSTR lpszFileName,
		LPWSTR lpszVolumePathName,
		DWORD cchBufferLength)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::GetVolumePathNameW));

		BOOL result = Cache::FileApi::GetVolumePathNameW(
			lpszFileName,
			lpszVolumePathName,
			cchBufferLength);

		message.AppendValue(lpszFileName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI LocalFileTimeToFileTime(
		const FILETIME *lpLocalFileTime,
		LPFILETIME lpFileTime)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LocalFileTimeToFileTime));

		BOOL result = Cache::FileApi::LocalFileTimeToFileTime(lpLocalFileTime, lpFileTime);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI LockFile(
		HANDLE hFile,
		DWORD dwFileOffsetLow,
		DWORD dwFileOffsetHigh,
		DWORD nNumberOfBytesToLockLow,
		DWORD nNumberOfBytesToLockHigh)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LockFile));

		BOOL result = Cache::FileApi::LockFile(
			hFile,
			dwFileOffsetLow,
			dwFileOffsetHigh,
			nNumberOfBytesToLockLow,
			nNumberOfBytesToLockHigh);

		message.AppendValue(result);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LockFileEx));

		BOOL result = Cache::FileApi::LockFileEx(
			hFile,
			dwFlags,
			dwReserved,
			nNumberOfBytesToLockLow,
			nNumberOfBytesToLockHigh,
			lpOverlapped);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI QueryDosDeviceW(
		LPCWSTR lpDeviceName,
		LPWSTR lpTargetPath,
		DWORD ucchMax)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::QueryDosDeviceW));

		DWORD result = Cache::FileApi::QueryDosDeviceW(
			lpDeviceName,
			lpTargetPath,
			ucchMax);

		message.AppendValue(lpDeviceName);
		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI ReadFile(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::ReadFile));

		BOOL result = Cache::FileApi::ReadFile(
			hFile,
			lpBuffer,
			nNumberOfBytesToRead,
			lpNumberOfBytesRead,
			lpOverlapped);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI ReadFileEx(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::ReadFileEx));

		BOOL result = Cache::FileApi::ReadFileEx(
			hFile,
			lpBuffer,
			nNumberOfBytesToRead,
			lpOverlapped,
			lpCompletionRoutine);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI ReadFileScatter(
		HANDLE hFile,
		FILE_SEGMENT_ELEMENT aSegmentArray[],
		DWORD nNumberOfBytesToRead,
		LPDWORD lpReserved,
		LPOVERLAPPED lpOverlapped)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::ReadFileScatter));

		BOOL result = Cache::FileApi::ReadFileScatter(
			hFile,
			aSegmentArray,
			nNumberOfBytesToRead,
			lpReserved,
			lpOverlapped);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI RemoveDirectoryA(
		LPCSTR lpPathName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::RemoveDirectoryA));

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
			result = Cache::FileApi::RemoveDirectoryA(lpPathName);
		}

		message.AppendValue(lpPathName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI RemoveDirectoryW(
		LPCWSTR lpPathName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::RemoveDirectoryW));

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
			result = Cache::FileApi::RemoveDirectoryW(lpPathName);
		}

		message.AppendValue(lpPathName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI SetEndOfFile(
		HANDLE hFile)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetEndOfFile));

		BOOL result = Cache::FileApi::SetEndOfFile(hFile);

		message.AppendValue(result);

		return result;
	}

	void WINAPI SetFileApisToANSI()
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileApisToANSI));

		Cache::FileApi::SetFileApisToANSI();
	}

	void WINAPI SetFileApisToOEM()
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileApisToOEM));

		Cache::FileApi::SetFileApisToOEM();
	}

	BOOL WINAPI SetFileAttributesA(
		LPCSTR lpFileName,
		DWORD  dwFileAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileAttributesA));

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
			result = Cache::FileApi::SetFileAttributesA(
				lpFileName,
				dwFileAttributes);
		}

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI SetFileAttributesW(
		LPCWSTR lpFileName,
		DWORD  dwFileAttributes)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileAttributesW));

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
			result = Cache::FileApi::SetFileAttributesW(
				lpFileName,
				dwFileAttributes);
		}

		message.AppendValue(lpFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}

	BOOL WINAPI SetFileInformationByHandle(
		HANDLE hFile,
		FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
		LPVOID lpFileInformation,
		DWORD dwBufferSize)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileInformationByHandle));

		BOOL result = Cache::FileApi::SetFileInformationByHandle(
			hFile,
			FileInformationClass,
			lpFileInformation,
			dwBufferSize);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFileIoOverlappedRange(
		HANDLE FileHandle,
		PUCHAR OverlappedRangeStart,
		ULONG Length)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileIoOverlappedRange));

		BOOL result = Cache::FileApi::SetFileIoOverlappedRange(
			FileHandle,
			OverlappedRangeStart,
			Length);

		message.AppendValue(result);

		return result;
	}

	DWORD WINAPI SetFilePointer(
		HANDLE hFile,
		LONG lDistanceToMove,
		PLONG lpDistanceToMoveHigh,
		DWORD dwMoveMethod)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFilePointer));

		DWORD result = Cache::FileApi::SetFilePointer(
			hFile,
			lDistanceToMove,
			lpDistanceToMoveHigh,
			dwMoveMethod);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFilePointerEx(
		HANDLE hFile,
		LARGE_INTEGER liDistanceToMove,
		PLARGE_INTEGER lpNewFilePointer,
		DWORD dwMoveMethod)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFilePointerEx));

		BOOL result = Cache::FileApi::SetFilePointerEx(
			hFile,
			liDistanceToMove,
			lpNewFilePointer,
			dwMoveMethod);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFileTime(
		HANDLE hFile,
		const FILETIME *lpCreationTime,
		const FILETIME *lpLastAccessTime,
		const FILETIME *lpLastWriteTime)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileTime));

		BOOL result = Cache::FileApi::SetFileTime(
			hFile,
			lpCreationTime,
			lpLastAccessTime,
			lpLastWriteTime);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI SetFileValidData(
		HANDLE   hFile,
		LONGLONG ValidDataLength)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SetFileValidData));

		BOOL result = Cache::FileApi::SetFileValidData(hFile, ValidDataLength);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI UnlockFile(
		HANDLE hFile,
		DWORD dwFileOffsetLow,
		DWORD dwFileOffsetHigh,
		DWORD nNumberOfBytesToUnlockLow,
		DWORD nNumberOfBytesToUnlockHigh)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::UnlockFile));

		BOOL result = Cache::FileApi::UnlockFile(
			hFile,
			dwFileOffsetLow,
			dwFileOffsetHigh,
			nNumberOfBytesToUnlockLow,
			nNumberOfBytesToUnlockHigh);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI UnlockFileEx(
		HANDLE hFile,
		DWORD dwReserved,
		DWORD nNumberOfBytesToUnlockLow,
		DWORD nNumberOfBytesToUnlockHigh,
		LPOVERLAPPED lpOverlapped)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::UnlockFileEx));

		BOOL result = Cache::FileApi::UnlockFileEx(
			hFile,
			dwReserved,
			nNumberOfBytesToUnlockLow,
			nNumberOfBytesToUnlockHigh,
			lpOverlapped);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI WriteFile(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten,
		LPOVERLAPPED lpOverlapped)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::WriteFile));

		BOOL result = Cache::FileApi::WriteFile(
			hFile,
			lpBuffer,
			nNumberOfBytesToWrite,
			lpNumberOfBytesWritten,
			lpOverlapped);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI WriteFileEx(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::WriteFileEx));

		BOOL result = Cache::FileApi::WriteFileEx(
			hFile,
			lpBuffer,
			nNumberOfBytesToWrite,
			lpOverlapped,
			lpCompletionRoutine);

		message.AppendValue(result);

		return result;
	}

	BOOL WINAPI WriteFileGather(
		HANDLE hFile,
		FILE_SEGMENT_ELEMENT aSegmentArray[],
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpReserved,
		LPOVERLAPPED lpOverlapped)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::WriteFileGather));

		BOOL result = Cache::FileApi::WriteFileGather(
			hFile,
			aSegmentArray,
			nNumberOfBytesToWrite,
			lpReserved,
			lpOverlapped);

		message.AppendValue(result);

		return result;
	}
}