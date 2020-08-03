module;

#include <windows.h>

#ifdef CreateProcess
#undef CreateProcess
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#pragma warning(push)
#if _MSC_VER > 1400
#pragma warning(disable:6102 6103)
#endif
#include <strsafe.h>
#pragma warning(pop)

#include <atomic>
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

export module Monitor.Shared;
import Detours;
import Opal;

using namespace Opal;

export constexpr const char* TBLOG_PIPE_NAMEA = "\\\\.\\pipe\\tracebuild";
export constexpr const wchar_t* TBLOG_PIPE_NAMEW = L"\\\\.\\pipe\\tracebuild";
#ifdef UNICODE
export constexpr const char* TBLOG_PIPE_NAME = TBLOG_PIPE_NAMEW;
#else
export constexpr const char* TBLOG_PIPE_NAME = TBLOG_PIPE_NAMEA;
#endif

namespace Monitor
{
	export enum class DetourMessageType : uint32_t
	{
		Info_Shutdown,
		Info_Error,

		// FileApi
		AreFileApisANSI,
		CompareFileTime,
		CreateDirectoryA,
		CreateDirectoryW,
		CreateFile2,
		CreateFileA,
		CreateFileW,
		DefineDosDeviceW,
		DeleteFileA,
		DeleteFileW,
		DeleteVolumeMountPointW,
		FileTimeToLocalFileTime,
		FindClose,
		FindCloseChangeNotification,
		FindFirstChangeNotificationA,
		FindFirstChangeNotificationW,
		FindFirstFileA,
		FindFirstFileW,
		FindFirstFileExA,
		FindFirstFileExW,
		FindFirstFileNameW,
		FindFirstStreamW,
		FindFirstVolumeW,
		FindNextChangeNotification,
		FindNextFileA,
		FindNextFileW,
		FindNextFileNameW,
		FindNextStreamW,
		FindNextVolumeW,
		FindVolumeClose,
		FlushFileBuffers,
		GetCompressedFileSizeA,
		GetCompressedFileSizeW,
		GetDiskFreeSpaceA,
		GetDiskFreeSpaceW,
		GetDiskFreeSpaceExA,
		GetDiskFreeSpaceExW,
		GetDriveTypeA,
		GetDriveTypeW,
		GetFileAttributesA,
		GetFileAttributesW,
		GetFileAttributesExA,
		GetFileAttributesExW,
		GetFileInformationByHandle,
		GetFileSize,
		GetFileSizeEx,
		GetFileTime,
		GetFileType,
		GetFinalPathNameByHandleA,
		GetFinalPathNameByHandleW,
		GetFullPathNameA,
		GetFullPathNameW,
		GetLogicalDrives,
		GetLogicalDriveStringsW,
		GetLongPathNameA,
		GetLongPathNameW,
		GetShortPathNameW,
		GetTempFileNameA,
		GetTempFileNameW,
		GetTempPathA,
		GetTempPathW,
		GetVolumeInformationA,
		GetVolumeInformationW,
		GetVolumeInformationByHandleW,
		GetVolumeNameForVolumeMountPointW,
		GetVolumePathNamesForVolumeNameW,
		GetVolumePathNameW,
		LocalFileTimeToFileTime,
		LockFile,
		LockFileEx,
		QueryDosDeviceW,
		ReadFile,
		ReadFileEx,
		ReadFileScatter,
		RemoveDirectoryA,
		RemoveDirectoryW,
		SetEndOfFile,
		SetFileApisToANSI,
		SetFileApisToOEM,
		SetFileAttributesA,
		SetFileAttributesW,
		SetFileInformationByHandle,
		SetFileIoOverlappedRange,
		SetFilePointer,
		SetFilePointerEx,
		SetFileTime,
		SetFileValidData,
		UnlockFile,
		UnlockFileEx,
		WriteFile,
		WriteFileEx,
		WriteFileGather,

		// LibLoaderApi
		LoadLibraryA,
		LoadLibraryW,
		LoadLibraryExA,
		LoadLibraryExW,

		// ProcessEnv
		SearchPathA,
		SearchPathW,

		// ProcessThreadsApi
		CreateProcessA,
		CreateProcessW,
		CreateProcessAsUserA,
		CreateProcessAsUserW,
		ExitProcess,

		// UndocumentedApi
		PrivCopyFileExA,
		PrivCopyFileExW,

		// WinBase
		CopyFileA,
		CopyFileW,
		CopyFile2,
		CopyFileExA,
		CopyFileExW,
		CopyFileTransactedA,
		CopyFileTransactedW,
		CreateDirectoryExA,
		CreateDirectoryExW,
		CreateDirectoryTransactedA,
		CreateDirectoryTransactedW,
		CreateFileTransactedA,
		CreateFileTransactedW,
		CreateHardLinkA,
		CreateHardLinkW,
		CreateHardLinkTransactedA,
		CreateHardLinkTransactedW,
		CreateProcessWithLogonW,
		CreateProcessWithTokenW,
		CreateSymbolicLinkA,
		CreateSymbolicLinkW,
		CreateSymbolicLinkTransactedA,
		CreateSymbolicLinkTransactedW,
		DecryptFileA,
		DecryptFileW,
		DeleteFileTransactedA,
		DeleteFileTransactedW,
		EncryptFileA,
		EncryptFileW,
		FileEncryptionStatusA,
		FileEncryptionStatusW,
		FindFirstFileNameTransactedW,
		FindFirstFileTransactedA,
		FindFirstFileTransactedW,
		FindFirstStreamTransactedW,
		GetBinaryTypeA,
		GetBinaryTypeW,
		GetCompressedFileSizeTransactedA,
		GetCompressedFileSizeTransactedW,
		GetDllDirectoryA,
		GetDllDirectoryW,
		GetFileAttributesTransactedA,
		GetFileAttributesTransactedW,
		GetFileBandwidthReservation,
		GetFileInformationByHandleEx,
		GetFileSecurityA,
		GetFullPathNameTransactedA,
		GetFullPathNameTransactedW,
		GetLongPathNameTransactedA,
		GetLongPathNameTransactedW,
		GetQueuedCompletionStatus,
		GetQueuedCompletionStatusEx,
		GetShortPathNameA,
		LoadModule,
		LoadPackagedLibrary,
		MoveFileA,
		MoveFileW,
		MoveFileExA,
		MoveFileExW,
		MoveFileTransactedA,
		MoveFileTransactedW,
		MoveFileWithProgressA,
		MoveFileWithProgressW,
		OpenEncryptedFileRawA,
		OpenEncryptedFileRawW,
		OpenFile,
		OpenFileById,
		ReadEncryptedFileRaw,
		RemoveDirectoryTransactedA,
		RemoveDirectoryTransactedW,
		ReOpenFile,
		ReplaceFileA,
		ReplaceFileW,
		SetCurrentDirectoryA,
		SetCurrentDirectoryW,
		SetDllDirectoryA,
		SetDllDirectoryW,
		SetFileAttributesTransactedA,
		SetFileAttributesTransactedW,
		SetFileBandwidthReservation,
		SetFileCompletionNotificationModes,
		SetFileSecurityA,
		SetFileShortNameA,
		SetFileShortNameW,
		SetSearchPathMode,
		WriteEncryptedFileRaw,
	};

	export struct DetourMessage
	{
		DetourMessageType Type;
		uint32_t ContentSize;
		byte Content[2048 - sizeof(Type) - sizeof(ContentSize)];
	};

	export struct DetourPayload
	{
		DWORD nParentProcessId;
		DWORD nTraceProcessId;
		DWORD nGeneology;
		DWORD rGeneology[64];
		WCHAR wzParents[256];
	};

	// Shared state payload guid.
	//
	export constexpr GUID GuidTrace = { 0xd8e2dc69, 0x3004, 0x453e, { 0x94, 0x15, 0x19, 0x0e, 0x79, 0xe8, 0x93, 0x52 } };
}

export void ThrowIfFailed(int32_t result, std::string_view message)
{
	if (result != NO_ERROR)
	{
		throw std::runtime_error(message.data());
	}
}

#include "MockDetourProcessManager.h"
#include "WindowsDetourProcessManager.h"