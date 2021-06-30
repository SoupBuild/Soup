
// TODO: Warning unsafe method
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <algorithm>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>

#include <windows.h>
#include <wofapi.h>

#ifdef CreateProcess
#undef CreateProcess
#endif

#include <stdio.h>
#include <strsafe.h>

#define DllExport __declspec(dllexport)

import Opal;
import Detours;
import Monitor.Shared;

#include "Helpers.h"

#include "Functions/FileApiOverrides.h"
#include "Functions/LibLoaderApiOverrides.h"
#include "Functions/ProcessEnvOverrides.h"
#include "Functions/ProcessThreadsApiOverrides.h"
#include "Functions/UndocumentedApiOverrides.h"
#include "Functions/WinBaseOverrides.h"

// TODO: This exported method forces a lib to be generated to allow for linking...
// Soup should allow runtime references to dlls that have no external symbols
DllExport void Noop()
{
}

void AttachDetours()
{
	ThrowIfFailed(DetourTransactionBegin(), "AttachDetours DetourTransactionBegin Failed");
	ThrowIfFailed(DetourUpdateThread(GetCurrentThread()), "AttachDetours DetourUpdateThread Failed");

	// FileApi
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::AreFileApisANSI, Functions::FileApi::Overrides::AreFileApisANSI), "AttachDetours DetourAttach AreFileApisANSI Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::CompareFileTime, Functions::FileApi::Overrides::CompareFileTime), "AttachDetours DetourAttach CompareFileTime Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::CreateDirectoryA, Functions::FileApi::Overrides::CreateDirectoryA), "AttachDetours DetourAttach CreateDirectoryA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::CreateDirectoryW, Functions::FileApi::Overrides::CreateDirectoryW), "AttachDetours DetourAttach CreateDirectoryW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::CreateFile2, Functions::FileApi::Overrides::CreateFile2), "AttachDetours DetourAttach CreateFile2 Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::CreateFileA, Functions::FileApi::Overrides::CreateFileA), "AttachDetours DetourAttach CreateFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::CreateFileW, Functions::FileApi::Overrides::CreateFileW), "AttachDetours DetourAttach CreateFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::DefineDosDeviceW, Functions::FileApi::Overrides::DefineDosDeviceW), "AttachDetours DetourAttach DefineDosDeviceW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::DeleteFileA, Functions::FileApi::Overrides::DeleteFileA), "AttachDetours DetourAttach DeleteFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::DeleteFileW, Functions::FileApi::Overrides::DeleteFileW), "AttachDetours DetourAttach DeleteFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::DeleteVolumeMountPointW, Functions::FileApi::Overrides::DeleteVolumeMountPointW), "AttachDetours DetourAttach DeleteVolumeMountPointW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FileTimeToLocalFileTime, Functions::FileApi::Overrides::FileTimeToLocalFileTime), "AttachDetours DetourAttach FileTimeToLocalFileTime Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindClose, Functions::FileApi::Overrides::FindClose), "AttachDetours DetourAttach FindClose Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindCloseChangeNotification, Functions::FileApi::Overrides::FindCloseChangeNotification), "AttachDetours DetourAttach FindCloseChangeNotification Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstChangeNotificationA, Functions::FileApi::Overrides::FindFirstChangeNotificationA), "AttachDetours DetourAttach FindFirstChangeNotificationA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstChangeNotificationW, Functions::FileApi::Overrides::FindFirstChangeNotificationW), "AttachDetours DetourAttach FindFirstChangeNotificationW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileA, Functions::FileApi::Overrides::FindFirstFileA), "AttachDetours DetourAttach FindFirstFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileW, Functions::FileApi::Overrides::FindFirstFileW), "AttachDetours DetourAttach FindFirstFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileExA, Functions::FileApi::Overrides::FindFirstFileExA), "AttachDetours DetourAttach FindFirstFileExA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileExW, Functions::FileApi::Overrides::FindFirstFileExW), "AttachDetours DetourAttach FindFirstFileExW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileNameW, Functions::FileApi::Overrides::FindFirstFileNameW), "AttachDetours DetourAttach FindFirstFileNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstStreamW, Functions::FileApi::Overrides::FindFirstStreamW), "AttachDetours DetourAttach FindFirstStreamW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindFirstVolumeW, Functions::FileApi::Overrides::FindFirstVolumeW), "AttachDetours DetourAttach FindFirstVolumeW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindNextChangeNotification, Functions::FileApi::Overrides::FindNextChangeNotification), "AttachDetours DetourAttach FindNextChangeNotification Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindNextFileA, Functions::FileApi::Overrides::FindNextFileA), "AttachDetours DetourAttach FindNextFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindNextFileW, Functions::FileApi::Overrides::FindNextFileW), "AttachDetours DetourAttach FindNextFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindNextFileNameW, Functions::FileApi::Overrides::FindNextFileNameW), "AttachDetours DetourAttach FindNextFileNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindNextStreamW, Functions::FileApi::Overrides::FindNextStreamW), "AttachDetours DetourAttach FindNextStreamW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindNextVolumeW, Functions::FileApi::Overrides::FindNextVolumeW), "AttachDetours DetourAttach FindNextVolumeW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FindVolumeClose, Functions::FileApi::Overrides::FindVolumeClose), "AttachDetours DetourAttach FindVolumeClose Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::FlushFileBuffers, Functions::FileApi::Overrides::FlushFileBuffers), "AttachDetours DetourAttach FlushFileBuffers Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetCompressedFileSizeA, Functions::FileApi::Overrides::GetCompressedFileSizeA), "AttachDetours DetourAttach GetCompressedFileSizeA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetCompressedFileSizeW, Functions::FileApi::Overrides::GetCompressedFileSizeW), "AttachDetours DetourAttach GetCompressedFileSizeW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetDiskFreeSpaceA, Functions::FileApi::Overrides::GetDiskFreeSpaceA), "AttachDetours DetourAttach GetDiskFreeSpaceA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetDiskFreeSpaceW, Functions::FileApi::Overrides::GetDiskFreeSpaceW), "AttachDetours DetourAttach GetDiskFreeSpaceW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetDiskFreeSpaceExA, Functions::FileApi::Overrides::GetDiskFreeSpaceExA), "AttachDetours DetourAttach GetDiskFreeSpaceExA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetDiskFreeSpaceExW, Functions::FileApi::Overrides::GetDiskFreeSpaceExW), "AttachDetours DetourAttach GetDiskFreeSpaceExW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetDriveTypeA, Functions::FileApi::Overrides::GetDriveTypeA), "AttachDetours DetourAttach GetDriveTypeA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetDriveTypeW, Functions::FileApi::Overrides::GetDriveTypeW), "AttachDetours DetourAttach GetDriveTypeW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileAttributesA, Functions::FileApi::Overrides::GetFileAttributesA), "AttachDetours DetourAttach GetFileAttributesA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileAttributesW, Functions::FileApi::Overrides::GetFileAttributesW), "AttachDetours DetourAttach GetFileAttributesW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileAttributesExA, Functions::FileApi::Overrides::GetFileAttributesExA), "AttachDetours DetourAttach GetFileAttributesExA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileAttributesExW, Functions::FileApi::Overrides::GetFileAttributesExW), "AttachDetours DetourAttach GetFileAttributesExW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileInformationByHandle, Functions::FileApi::Overrides::GetFileInformationByHandle), "AttachDetours DetourAttach GetFileInformationByHandle Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileSize, Functions::FileApi::Overrides::GetFileSize), "AttachDetours DetourAttach GetFileSize Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileSizeEx, Functions::FileApi::Overrides::GetFileSizeEx), "AttachDetours DetourAttach GetFileSizeEx Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileTime, Functions::FileApi::Overrides::GetFileTime), "AttachDetours DetourAttach GetFileTime Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFileType, Functions::FileApi::Overrides::GetFileType), "AttachDetours DetourAttach GetFileType Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFinalPathNameByHandleA, Functions::FileApi::Overrides::GetFinalPathNameByHandleA), "AttachDetours DetourAttach GetFinalPathNameByHandleA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFinalPathNameByHandleW, Functions::FileApi::Overrides::GetFinalPathNameByHandleW), "AttachDetours DetourAttach GetFinalPathNameByHandleW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFullPathNameA, Functions::FileApi::Overrides::GetFullPathNameA), "AttachDetours DetourAttach GetFullPathNameA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetFullPathNameW, Functions::FileApi::Overrides::GetFullPathNameW), "AttachDetours DetourAttach GetFullPathNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetLogicalDrives, Functions::FileApi::Overrides::GetLogicalDrives), "AttachDetours DetourAttach GetLogicalDrives Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetLogicalDriveStringsW, Functions::FileApi::Overrides::GetLogicalDriveStringsW), "AttachDetours DetourAttach GetLogicalDriveStringsW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetLongPathNameA, Functions::FileApi::Overrides::GetLongPathNameA), "AttachDetours DetourAttach GetLongPathNameA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetLongPathNameW, Functions::FileApi::Overrides::GetLongPathNameW), "AttachDetours DetourAttach GetLongPathNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetShortPathNameW, Functions::FileApi::Overrides::GetShortPathNameW), "AttachDetours DetourAttach GetShortPathNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetTempFileNameA, Functions::FileApi::Overrides::GetTempFileNameA), "AttachDetours DetourAttach GetTempFileNameA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetTempFileNameW, Functions::FileApi::Overrides::GetTempFileNameW), "AttachDetours DetourAttach GetTempFileNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetTempPathA, Functions::FileApi::Overrides::GetTempPathA), "AttachDetours DetourAttach GetTempPathA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetTempPathW, Functions::FileApi::Overrides::GetTempPathW), "AttachDetours DetourAttach GetTempPathW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetVolumeInformationA, Functions::FileApi::Overrides::GetVolumeInformationA), "AttachDetours DetourAttach GetVolumeInformationA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetVolumeInformationW, Functions::FileApi::Overrides::GetVolumeInformationW), "AttachDetours DetourAttach GetVolumeInformationW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetVolumeInformationByHandleW, Functions::FileApi::Overrides::GetVolumeInformationByHandleW), "AttachDetours DetourAttach GetVolumeInformationByHandleW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetVolumeNameForVolumeMountPointW, Functions::FileApi::Overrides::GetVolumeNameForVolumeMountPointW), "AttachDetours DetourAttach GetVolumeNameForVolumeMountPointW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetVolumePathNamesForVolumeNameW, Functions::FileApi::Overrides::GetVolumePathNamesForVolumeNameW), "AttachDetours DetourAttach GetVolumePathNamesForVolumeNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::GetVolumePathNameW, Functions::FileApi::Overrides::GetVolumePathNameW), "AttachDetours DetourAttach GetVolumePathNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::LocalFileTimeToFileTime, Functions::FileApi::Overrides::LocalFileTimeToFileTime), "AttachDetours DetourAttach LocalFileTimeToFileTime Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::LockFile, Functions::FileApi::Overrides::LockFile), "AttachDetours DetourAttach LockFile Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::LockFileEx, Functions::FileApi::Overrides::LockFileEx), "AttachDetours DetourAttach LockFileEx Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::QueryDosDeviceW, Functions::FileApi::Overrides::QueryDosDeviceW), "AttachDetours DetourAttach QueryDosDeviceW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::ReadFile, Functions::FileApi::Overrides::ReadFile), "AttachDetours DetourAttach ReadFile Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::ReadFileEx, Functions::FileApi::Overrides::ReadFileEx), "AttachDetours DetourAttach ReadFileEx Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::ReadFileScatter, Functions::FileApi::Overrides::ReadFileScatter), "AttachDetours DetourAttach ReadFileScatter Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::RemoveDirectoryA, Functions::FileApi::Overrides::RemoveDirectoryA), "AttachDetours DetourAttach RemoveDirectoryA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::RemoveDirectoryW, Functions::FileApi::Overrides::RemoveDirectoryW), "AttachDetours DetourAttach RemoveDirectoryW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetEndOfFile, Functions::FileApi::Overrides::SetEndOfFile), "AttachDetours DetourAttach SetEndOfFile Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFileApisToANSI, Functions::FileApi::Overrides::SetFileApisToANSI), "AttachDetours DetourAttach SetFileApisToANSI Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFileApisToOEM, Functions::FileApi::Overrides::SetFileApisToOEM), "AttachDetours DetourAttach SetFileApisToOEM Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFileAttributesA, Functions::FileApi::Overrides::SetFileAttributesA), "AttachDetours DetourAttach SetFileAttributesA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFileAttributesW, Functions::FileApi::Overrides::SetFileAttributesW), "AttachDetours DetourAttach SetFileAttributesW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFileInformationByHandle, Functions::FileApi::Overrides::SetFileInformationByHandle), "AttachDetours DetourAttach SetFileInformationByHandle Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFileIoOverlappedRange, Functions::FileApi::Overrides::SetFileIoOverlappedRange), "AttachDetours DetourAttach SetFileIoOverlappedRange Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFilePointer, Functions::FileApi::Overrides::SetFilePointer), "AttachDetours DetourAttach SetFilePointer Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFilePointerEx, Functions::FileApi::Overrides::SetFilePointerEx), "AttachDetours DetourAttach SetFilePointerEx Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFileTime, Functions::FileApi::Overrides::SetFileTime), "AttachDetours DetourAttach SetFileTime Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::SetFileValidData, Functions::FileApi::Overrides::SetFileValidData), "AttachDetours DetourAttach SetFileValidData Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::UnlockFile, Functions::FileApi::Overrides::UnlockFile), "AttachDetours DetourAttach UnlockFile Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::UnlockFileEx, Functions::FileApi::Overrides::UnlockFileEx), "AttachDetours DetourAttach UnlockFileEx Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::WriteFile, Functions::FileApi::Overrides::WriteFile), "AttachDetours DetourAttach WriteFile Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::WriteFileEx, Functions::FileApi::Overrides::WriteFileEx), "AttachDetours DetourAttach WriteFileEx Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::FileApi::Cache::WriteFileGather, Functions::FileApi::Overrides::WriteFileGather), "AttachDetours DetourAttach WriteFileGather Failed");

	// LibLoaderApi
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::LibLoaderApi::Cache::LoadLibraryA, Functions::LibLoaderApi::Overrides::LoadLibraryA), "AttachDetours DetourAttach LoadLibraryA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::LibLoaderApi::Cache::LoadLibraryW, Functions::LibLoaderApi::Overrides::LoadLibraryW), "AttachDetours DetourAttach LoadLibraryW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::LibLoaderApi::Cache::LoadLibraryExA, Functions::LibLoaderApi::Overrides::LoadLibraryExA), "AttachDetours DetourAttach LoadLibraryExA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::LibLoaderApi::Cache::LoadLibraryExW, Functions::LibLoaderApi::Overrides::LoadLibraryExW), "AttachDetours DetourAttach LoadLibraryExW Failed");

	// ProcessEnv
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::ProcessEnv::Cache::SearchPathA, Functions::ProcessEnv::Overrides::SearchPathA), "AttachDetours DetourAttach SearchPathA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::ProcessEnv::Cache::SearchPathW, Functions::ProcessEnv::Overrides::SearchPathW), "AttachDetours DetourAttach SearchPathW Failed");

	// ProcessThreadsApi
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::ProcessThreadsApi::Cache::CreateProcessA, Functions::ProcessThreadsApi::Overrides::CreateProcessA), "AttachDetours DetourAttach CreateProcessA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::ProcessThreadsApi::Cache::CreateProcessW, Functions::ProcessThreadsApi::Overrides::CreateProcessW), "AttachDetours DetourAttach CreateProcessW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::ProcessThreadsApi::Cache::CreateProcessAsUserA, Functions::ProcessThreadsApi::Overrides::CreateProcessAsUserA), "AttachDetours DetourAttach CreateProcessAsUserA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::ProcessThreadsApi::Cache::CreateProcessAsUserW, Functions::ProcessThreadsApi::Overrides::CreateProcessAsUserW), "AttachDetours DetourAttach CreateProcessAsUserW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::ProcessThreadsApi::Cache::ExitProcess, Functions::ProcessThreadsApi::Overrides::ExitProcess), "AttachDetours DetourAttach ExitProcess Failed");

	// UndocumentedApi
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::UndocumentedApi::Cache::EntryPoint, Functions::UndocumentedApi::Overrides::EntryPoint), "AttachDetours DetourAttach EntryPoint Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::UndocumentedApi::Cache::PrivCopyFileExA, Functions::UndocumentedApi::Overrides::PrivCopyFileExA), "AttachDetours DetourAttach PrivCopyFileExA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::UndocumentedApi::Cache::PrivCopyFileExW, Functions::UndocumentedApi::Overrides::PrivCopyFileExW), "AttachDetours DetourAttach PrivCopyFileExW Failed");

	// WinBase
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CopyFileA, Functions::WinBase::Overrides::CopyFileA), "AttachDetours DetourAttach CopyFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CopyFileW, Functions::WinBase::Overrides::CopyFileW), "AttachDetours DetourAttach CopyFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CopyFile2, Functions::WinBase::Overrides::CopyFile2), "AttachDetours DetourAttach CopyFile2 Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CopyFileExA, Functions::WinBase::Overrides::CopyFileExA), "AttachDetours DetourAttach CopyFileExA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CopyFileExW, Functions::WinBase::Overrides::CopyFileExW), "AttachDetours DetourAttach CopyFileExW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CopyFileTransactedA, Functions::WinBase::Overrides::CopyFileTransactedA), "AttachDetours DetourAttach CopyFileTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CopyFileTransactedW, Functions::WinBase::Overrides::CopyFileTransactedW), "AttachDetours DetourAttach CopyFileTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateDirectoryExA, Functions::WinBase::Overrides::CreateDirectoryExA), "AttachDetours DetourAttach CreateDirectoryExA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateDirectoryExW, Functions::WinBase::Overrides::CreateDirectoryExW), "AttachDetours DetourAttach CreateDirectoryExW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateDirectoryTransactedA, Functions::WinBase::Overrides::CreateDirectoryTransactedA), "AttachDetours DetourAttach CreateDirectoryTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateDirectoryTransactedW, Functions::WinBase::Overrides::CreateDirectoryTransactedW), "AttachDetours DetourAttach CreateDirectoryTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateFileTransactedA, Functions::WinBase::Overrides::CreateFileTransactedA), "AttachDetours DetourAttach CreateFileTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateFileTransactedW, Functions::WinBase::Overrides::CreateFileTransactedW), "AttachDetours DetourAttach CreateFileTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateHardLinkA, Functions::WinBase::Overrides::CreateHardLinkA), "AttachDetours DetourAttach CreateHardLinkA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateHardLinkW, Functions::WinBase::Overrides::CreateHardLinkW), "AttachDetours DetourAttach CreateHardLinkW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateHardLinkTransactedA, Functions::WinBase::Overrides::CreateHardLinkTransactedA), "AttachDetours DetourAttach CreateHardLinkTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateHardLinkTransactedW, Functions::WinBase::Overrides::CreateHardLinkTransactedW), "AttachDetours DetourAttach CreateHardLinkTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateProcessWithLogonW, Functions::WinBase::Overrides::CreateProcessWithLogonW), "AttachDetours DetourAttach CreateProcessWithLogonW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateProcessWithTokenW, Functions::WinBase::Overrides::CreateProcessWithTokenW), "AttachDetours DetourAttach CreateProcessWithTokenW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateSymbolicLinkA, Functions::WinBase::Overrides::CreateSymbolicLinkA), "AttachDetours DetourAttach CreateSymbolicLinkA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateSymbolicLinkW, Functions::WinBase::Overrides::CreateSymbolicLinkW), "AttachDetours DetourAttach CreateSymbolicLinkW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateSymbolicLinkTransactedA, Functions::WinBase::Overrides::CreateSymbolicLinkTransactedA), "AttachDetours DetourAttach CreateSymbolicLinkTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::CreateSymbolicLinkTransactedW, Functions::WinBase::Overrides::CreateSymbolicLinkTransactedW), "AttachDetours DetourAttach CreateSymbolicLinkTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::DecryptFileA, Functions::WinBase::Overrides::DecryptFileA), "AttachDetours DetourAttach DecryptFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::DecryptFileW, Functions::WinBase::Overrides::DecryptFileW), "AttachDetours DetourAttach DecryptFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::DeleteFileTransactedA, Functions::WinBase::Overrides::DeleteFileTransactedA), "AttachDetours DetourAttach DeleteFileTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::DeleteFileTransactedW, Functions::WinBase::Overrides::DeleteFileTransactedW), "AttachDetours DetourAttach DeleteFileTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::EncryptFileA, Functions::WinBase::Overrides::EncryptFileA), "AttachDetours DetourAttach EncryptFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::EncryptFileW, Functions::WinBase::Overrides::EncryptFileW), "AttachDetours DetourAttach EncryptFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::FileEncryptionStatusA, Functions::WinBase::Overrides::FileEncryptionStatusA), "AttachDetours DetourAttach FileEncryptionStatusA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::FileEncryptionStatusW, Functions::WinBase::Overrides::FileEncryptionStatusW), "AttachDetours DetourAttach FileEncryptionStatusW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::FindFirstFileNameTransactedW, Functions::WinBase::Overrides::FindFirstFileNameTransactedW), "AttachDetours DetourAttach FindFirstFileNameTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::FindFirstFileTransactedA, Functions::WinBase::Overrides::FindFirstFileTransactedA), "AttachDetours DetourAttach FindFirstFileTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::FindFirstFileTransactedW, Functions::WinBase::Overrides::FindFirstFileTransactedW), "AttachDetours DetourAttach FindFirstFileTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::FindFirstStreamTransactedW, Functions::WinBase::Overrides::FindFirstStreamTransactedW), "AttachDetours DetourAttach FindFirstStreamTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetBinaryTypeA, Functions::WinBase::Overrides::GetBinaryTypeA), "AttachDetours DetourAttach GetBinaryTypeA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetBinaryTypeW, Functions::WinBase::Overrides::GetBinaryTypeW), "AttachDetours DetourAttach GetBinaryTypeW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetCompressedFileSizeTransactedA, Functions::WinBase::Overrides::GetCompressedFileSizeTransactedA), "AttachDetours DetourAttach GetCompressedFileSizeTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetCompressedFileSizeTransactedW, Functions::WinBase::Overrides::GetCompressedFileSizeTransactedW), "AttachDetours DetourAttach GetCompressedFileSizeTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetDllDirectoryA, Functions::WinBase::Overrides::GetDllDirectoryA), "AttachDetours DetourAttach GetDllDirectoryA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetDllDirectoryW, Functions::WinBase::Overrides::GetDllDirectoryW), "AttachDetours DetourAttach GetDllDirectoryW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetFileAttributesTransactedA, Functions::WinBase::Overrides::GetFileAttributesTransactedA), "AttachDetours DetourAttach GetFileAttributesTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetFileAttributesTransactedW, Functions::WinBase::Overrides::GetFileAttributesTransactedW), "AttachDetours DetourAttach GetFileAttributesTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetFileBandwidthReservation, Functions::WinBase::Overrides::GetFileBandwidthReservation), "AttachDetours DetourAttach GetFileBandwidthReservation Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetFileInformationByHandleEx, Functions::WinBase::Overrides::GetFileInformationByHandleEx), "AttachDetours DetourAttach GetFileInformationByHandleEx Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetFileSecurityA, Functions::WinBase::Overrides::GetFileSecurityA), "AttachDetours DetourAttach GetFileSecurityA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetFullPathNameTransactedA, Functions::WinBase::Overrides::GetFullPathNameTransactedA), "AttachDetours DetourAttach GetFullPathNameTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetFullPathNameTransactedW, Functions::WinBase::Overrides::GetFullPathNameTransactedW), "AttachDetours DetourAttach GetFullPathNameTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetLongPathNameTransactedA, Functions::WinBase::Overrides::GetLongPathNameTransactedA), "AttachDetours DetourAttach GetLongPathNameTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetLongPathNameTransactedW, Functions::WinBase::Overrides::GetLongPathNameTransactedW), "AttachDetours DetourAttach GetLongPathNameTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetQueuedCompletionStatus, Functions::WinBase::Overrides::GetQueuedCompletionStatus), "AttachDetours DetourAttach GetQueuedCompletionStatus Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetQueuedCompletionStatusEx, Functions::WinBase::Overrides::GetQueuedCompletionStatusEx), "AttachDetours DetourAttach GetQueuedCompletionStatusEx Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::GetShortPathNameA, Functions::WinBase::Overrides::GetShortPathNameA), "AttachDetours DetourAttach GetShortPathNameA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::LoadModule, Functions::WinBase::Overrides::LoadModule), "AttachDetours DetourAttach LoadModule Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::LoadPackagedLibrary, Functions::WinBase::Overrides::LoadPackagedLibrary), "AttachDetours DetourAttach LoadPackagedLibrary Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::MoveFileA, Functions::WinBase::Overrides::MoveFileA), "AttachDetours DetourAttach MoveFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::MoveFileW, Functions::WinBase::Overrides::MoveFileW), "AttachDetours DetourAttach MoveFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::MoveFileExA, Functions::WinBase::Overrides::MoveFileExA), "AttachDetours DetourAttach MoveFileExA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::MoveFileExW, Functions::WinBase::Overrides::MoveFileExW), "AttachDetours DetourAttach MoveFileExW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::MoveFileTransactedA, Functions::WinBase::Overrides::MoveFileTransactedA), "AttachDetours DetourAttach MoveFileTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::MoveFileTransactedW, Functions::WinBase::Overrides::MoveFileTransactedW), "AttachDetours DetourAttach MoveFileTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::MoveFileWithProgressA, Functions::WinBase::Overrides::MoveFileWithProgressA), "AttachDetours DetourAttach MoveFileWithProgressA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::MoveFileWithProgressW, Functions::WinBase::Overrides::MoveFileWithProgressW), "AttachDetours DetourAttach MoveFileWithProgressW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::OpenEncryptedFileRawA, Functions::WinBase::Overrides::OpenEncryptedFileRawA), "AttachDetours DetourAttach OpenEncryptedFileRawA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::OpenEncryptedFileRawW, Functions::WinBase::Overrides::OpenEncryptedFileRawW), "AttachDetours DetourAttach OpenEncryptedFileRawW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::OpenFile, Functions::WinBase::Overrides::OpenFile), "AttachDetours DetourAttach OpenFile Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::OpenFileById, Functions::WinBase::Overrides::OpenFileById), "AttachDetours DetourAttach OpenFileById Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::ReadEncryptedFileRaw, Functions::WinBase::Overrides::ReadEncryptedFileRaw), "AttachDetours DetourAttach ReadEncryptedFileRaw Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::RemoveDirectoryTransactedA, Functions::WinBase::Overrides::RemoveDirectoryTransactedA), "AttachDetours DetourAttach RemoveDirectoryTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::RemoveDirectoryTransactedW, Functions::WinBase::Overrides::RemoveDirectoryTransactedW), "AttachDetours DetourAttach RemoveDirectoryTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::ReOpenFile, Functions::WinBase::Overrides::ReOpenFile), "AttachDetours DetourAttach ReOpenFile Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::ReplaceFileA, Functions::WinBase::Overrides::ReplaceFileA), "AttachDetours DetourAttach ReplaceFileA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::ReplaceFileW, Functions::WinBase::Overrides::ReplaceFileW), "AttachDetours DetourAttach ReplaceFileW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetCurrentDirectoryA, Functions::WinBase::Overrides::SetCurrentDirectoryA), "AttachDetours DetourAttach SetCurrentDirectoryA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetCurrentDirectoryW, Functions::WinBase::Overrides::SetCurrentDirectoryW), "AttachDetours DetourAttach SetCurrentDirectoryW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetDllDirectoryA, Functions::WinBase::Overrides::SetDllDirectoryA), "AttachDetours DetourAttach SetDllDirectoryA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetDllDirectoryW, Functions::WinBase::Overrides::SetDllDirectoryW), "AttachDetours DetourAttach SetDllDirectoryW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetFileAttributesTransactedA, Functions::WinBase::Overrides::SetFileAttributesTransactedA), "AttachDetours DetourAttach SetFileAttributesTransactedA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetFileAttributesTransactedW, Functions::WinBase::Overrides::SetFileAttributesTransactedW), "AttachDetours DetourAttach SetFileAttributesTransactedW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetFileBandwidthReservation, Functions::WinBase::Overrides::SetFileBandwidthReservation), "AttachDetours DetourAttach SetFileBandwidthReservation Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetFileCompletionNotificationModes, Functions::WinBase::Overrides::SetFileCompletionNotificationModes), "AttachDetours DetourAttach SetFileCompletionNotificationModes Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetFileSecurityA, Functions::WinBase::Overrides::SetFileSecurityA), "AttachDetours DetourAttach SetFileSecurityA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetFileShortNameA, Functions::WinBase::Overrides::SetFileShortNameA), "AttachDetours DetourAttach SetFileShortNameA Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetFileShortNameW, Functions::WinBase::Overrides::SetFileShortNameW), "AttachDetours DetourAttach SetFileShortNameW Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::SetSearchPathMode, Functions::WinBase::Overrides::SetSearchPathMode), "AttachDetours DetourAttach SetSearchPathMode Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::WinBase::Cache::WriteEncryptedFileRaw, Functions::WinBase::Overrides::WriteEncryptedFileRaw), "AttachDetours DetourAttach WriteEncryptedFileRaw Failed");

	ThrowIfFailed(DetourTransactionCommit(), "AttachDetours DetourTransactionCommit Failed");
}

void DetachDetours()
{
	ThrowIfFailed(DetourTransactionBegin(), "DetachDetours DetourTransactionBegin Failed");
	ThrowIfFailed(DetourUpdateThread(GetCurrentThread()), "DetachDetours DetourUpdateThread Failed");

	// FileApi
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::AreFileApisANSI, Functions::FileApi::Overrides::AreFileApisANSI), "DetachDetours DetourDetach AreFileApisANSI Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::CompareFileTime, Functions::FileApi::Overrides::CompareFileTime), "DetachDetours DetourDetach CompareFileTime Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::CreateDirectoryA, Functions::FileApi::Overrides::CreateDirectoryA), "DetachDetours DetourDetach CreateDirectoryA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::CreateDirectoryW, Functions::FileApi::Overrides::CreateDirectoryW), "DetachDetours DetourDetach CreateDirectoryW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::CreateFile2, Functions::FileApi::Overrides::CreateFile2), "DetachDetours DetourDetach CreateFile2 Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::CreateFileA, Functions::FileApi::Overrides::CreateFileA), "DetachDetours DetourDetach CreateFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::CreateFileW, Functions::FileApi::Overrides::CreateFileW), "DetachDetours DetourDetach CreateFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::DefineDosDeviceW, Functions::FileApi::Overrides::DefineDosDeviceW), "DetachDetours DetourDetach DefineDosDeviceW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::DeleteFileA, Functions::FileApi::Overrides::DeleteFileA), "DetachDetours DetourDetach DeleteFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::DeleteFileW, Functions::FileApi::Overrides::DeleteFileW), "DetachDetours DetourDetach DeleteFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::DeleteVolumeMountPointW, Functions::FileApi::Overrides::DeleteVolumeMountPointW), "DetachDetours DetourDetach DeleteVolumeMountPointW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FileTimeToLocalFileTime, Functions::FileApi::Overrides::FileTimeToLocalFileTime), "DetachDetours DetourDetach FileTimeToLocalFileTime Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindClose, Functions::FileApi::Overrides::FindClose), "DetachDetours DetourDetach FindClose Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindCloseChangeNotification, Functions::FileApi::Overrides::FindCloseChangeNotification), "DetachDetours DetourDetach FindCloseChangeNotification Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstChangeNotificationA, Functions::FileApi::Overrides::FindFirstChangeNotificationA), "DetachDetours DetourDetach FindFirstChangeNotificationA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstChangeNotificationW, Functions::FileApi::Overrides::FindFirstChangeNotificationW), "DetachDetours DetourDetach FindFirstChangeNotificationW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileA, Functions::FileApi::Overrides::FindFirstFileA), "DetachDetours DetourDetach FindFirstFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileW, Functions::FileApi::Overrides::FindFirstFileW), "DetachDetours DetourDetach FindFirstFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileExA, Functions::FileApi::Overrides::FindFirstFileExA), "DetachDetours DetourDetach FindFirstFileExA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileExW, Functions::FileApi::Overrides::FindFirstFileExW), "DetachDetours DetourDetach FindFirstFileExW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstFileNameW, Functions::FileApi::Overrides::FindFirstFileNameW), "DetachDetours DetourDetach FindFirstFileNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstStreamW, Functions::FileApi::Overrides::FindFirstStreamW), "DetachDetours DetourDetach FindFirstStreamW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindFirstVolumeW, Functions::FileApi::Overrides::FindFirstVolumeW), "DetachDetours DetourDetach FindFirstVolumeW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindNextChangeNotification, Functions::FileApi::Overrides::FindNextChangeNotification), "DetachDetours DetourDetach FindNextChangeNotification Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindNextFileA, Functions::FileApi::Overrides::FindNextFileA), "DetachDetours DetourDetach FindNextFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindNextFileW, Functions::FileApi::Overrides::FindNextFileW), "DetachDetours DetourDetach FindNextFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindNextFileNameW, Functions::FileApi::Overrides::FindNextFileNameW), "DetachDetours DetourDetach FindNextFileNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindNextStreamW, Functions::FileApi::Overrides::FindNextStreamW), "DetachDetours DetourDetach FindNextStreamW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindNextVolumeW, Functions::FileApi::Overrides::FindNextVolumeW), "DetachDetours DetourDetach FindNextVolumeW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FindVolumeClose, Functions::FileApi::Overrides::FindVolumeClose), "DetachDetours DetourDetach FindVolumeClose Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::FlushFileBuffers, Functions::FileApi::Overrides::FlushFileBuffers), "DetachDetours DetourDetach FlushFileBuffers Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetCompressedFileSizeA, Functions::FileApi::Overrides::GetCompressedFileSizeA), "DetachDetours DetourDetach GetCompressedFileSizeA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetCompressedFileSizeW, Functions::FileApi::Overrides::GetCompressedFileSizeW), "DetachDetours DetourDetach GetCompressedFileSizeW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetDiskFreeSpaceA, Functions::FileApi::Overrides::GetDiskFreeSpaceA), "DetachDetours DetourDetach GetDiskFreeSpaceA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetDiskFreeSpaceW, Functions::FileApi::Overrides::GetDiskFreeSpaceW), "DetachDetours DetourDetach GetDiskFreeSpaceW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetDiskFreeSpaceExA, Functions::FileApi::Overrides::GetDiskFreeSpaceExA), "DetachDetours DetourDetach GetDiskFreeSpaceExA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetDiskFreeSpaceExW, Functions::FileApi::Overrides::GetDiskFreeSpaceExW), "DetachDetours DetourDetach GetDiskFreeSpaceExW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetDriveTypeA, Functions::FileApi::Overrides::GetDriveTypeA), "DetachDetours DetourDetach GetDriveTypeA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetDriveTypeW, Functions::FileApi::Overrides::GetDriveTypeW), "DetachDetours DetourDetach GetDriveTypeW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileAttributesA, Functions::FileApi::Overrides::GetFileAttributesA), "DetachDetours DetourDetach GetFileAttributesA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileAttributesW, Functions::FileApi::Overrides::GetFileAttributesW), "DetachDetours DetourDetach GetFileAttributesW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileAttributesExA, Functions::FileApi::Overrides::GetFileAttributesExA), "DetachDetours DetourDetach GetFileAttributesExA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileAttributesExW, Functions::FileApi::Overrides::GetFileAttributesExW), "DetachDetours DetourDetach GetFileAttributesExW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileInformationByHandle, Functions::FileApi::Overrides::GetFileInformationByHandle), "DetachDetours DetourDetach GetFileInformationByHandle Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileSize, Functions::FileApi::Overrides::GetFileSize), "DetachDetours DetourDetach GetFileSize Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileSizeEx, Functions::FileApi::Overrides::GetFileSizeEx), "DetachDetours DetourDetach GetFileSizeEx Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileTime, Functions::FileApi::Overrides::GetFileTime), "DetachDetours DetourDetach GetFileTime Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFileType, Functions::FileApi::Overrides::GetFileType), "DetachDetours DetourDetach GetFileType Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFinalPathNameByHandleA, Functions::FileApi::Overrides::GetFinalPathNameByHandleA), "DetachDetours DetourDetach GetFinalPathNameByHandleA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFinalPathNameByHandleW, Functions::FileApi::Overrides::GetFinalPathNameByHandleW), "DetachDetours DetourDetach GetFinalPathNameByHandleW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFullPathNameA, Functions::FileApi::Overrides::GetFullPathNameA), "DetachDetours DetourDetach GetFullPathNameA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetFullPathNameW, Functions::FileApi::Overrides::GetFullPathNameW), "DetachDetours DetourDetach GetFullPathNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetLogicalDrives, Functions::FileApi::Overrides::GetLogicalDrives), "DetachDetours DetourDetach GetLogicalDrives Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetLogicalDriveStringsW, Functions::FileApi::Overrides::GetLogicalDriveStringsW), "DetachDetours DetourDetach GetLogicalDriveStringsW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetLongPathNameA, Functions::FileApi::Overrides::GetLongPathNameA), "DetachDetours DetourDetach GetLongPathNameA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetLongPathNameW, Functions::FileApi::Overrides::GetLongPathNameW), "DetachDetours DetourDetach GetLongPathNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetShortPathNameW, Functions::FileApi::Overrides::GetShortPathNameW), "DetachDetours DetourDetach GetShortPathNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetTempFileNameA, Functions::FileApi::Overrides::GetTempFileNameA), "DetachDetours DetourDetach GetTempFileNameA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetTempFileNameW, Functions::FileApi::Overrides::GetTempFileNameW), "DetachDetours DetourDetach GetTempFileNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetTempPathA, Functions::FileApi::Overrides::GetTempPathA), "DetachDetours DetourDetach GetTempPathA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetTempPathW, Functions::FileApi::Overrides::GetTempPathW), "DetachDetours DetourDetach GetTempPathW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetVolumeInformationA, Functions::FileApi::Overrides::GetVolumeInformationA), "DetachDetours DetourDetach GetVolumeInformationA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetVolumeInformationW, Functions::FileApi::Overrides::GetVolumeInformationW), "DetachDetours DetourDetach GetVolumeInformationW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetVolumeInformationByHandleW, Functions::FileApi::Overrides::GetVolumeInformationByHandleW), "DetachDetours DetourDetach GetVolumeInformationByHandleW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetVolumeNameForVolumeMountPointW, Functions::FileApi::Overrides::GetVolumeNameForVolumeMountPointW), "DetachDetours DetourDetach GetVolumeNameForVolumeMountPointW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetVolumePathNamesForVolumeNameW, Functions::FileApi::Overrides::GetVolumePathNamesForVolumeNameW), "DetachDetours DetourDetach GetVolumePathNamesForVolumeNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::GetVolumePathNameW, Functions::FileApi::Overrides::GetVolumePathNameW), "DetachDetours DetourDetach GetVolumePathNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::LocalFileTimeToFileTime, Functions::FileApi::Overrides::LocalFileTimeToFileTime), "DetachDetours DetourDetach LocalFileTimeToFileTime Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::LockFile, Functions::FileApi::Overrides::LockFile), "DetachDetours DetourDetach LockFile Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::LockFileEx, Functions::FileApi::Overrides::LockFileEx), "DetachDetours DetourDetach LockFileEx Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::QueryDosDeviceW, Functions::FileApi::Overrides::QueryDosDeviceW), "DetachDetours DetourDetach QueryDosDeviceW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::ReadFile, Functions::FileApi::Overrides::ReadFile), "DetachDetours DetourDetach ReadFile Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::ReadFileEx, Functions::FileApi::Overrides::ReadFileEx), "DetachDetours DetourDetach ReadFileEx Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::ReadFileScatter, Functions::FileApi::Overrides::ReadFileScatter), "DetachDetours DetourDetach ReadFileScatter Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::RemoveDirectoryA, Functions::FileApi::Overrides::RemoveDirectoryA), "DetachDetours DetourDetach RemoveDirectoryA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::RemoveDirectoryW, Functions::FileApi::Overrides::RemoveDirectoryW), "DetachDetours DetourDetach RemoveDirectoryW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetEndOfFile, Functions::FileApi::Overrides::SetEndOfFile), "DetachDetours DetourDetach SetEndOfFile Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFileApisToANSI, Functions::FileApi::Overrides::SetFileApisToANSI), "DetachDetours DetourDetach SetFileApisToANSI Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFileApisToOEM, Functions::FileApi::Overrides::SetFileApisToOEM), "DetachDetours DetourDetach SetFileApisToOEM Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFileAttributesA, Functions::FileApi::Overrides::SetFileAttributesA), "DetachDetours DetourDetach SetFileAttributesA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFileAttributesW, Functions::FileApi::Overrides::SetFileAttributesW), "DetachDetours DetourDetach SetFileAttributesW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFileInformationByHandle, Functions::FileApi::Overrides::SetFileInformationByHandle), "DetachDetours DetourDetach SetFileInformationByHandle Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFileIoOverlappedRange, Functions::FileApi::Overrides::SetFileIoOverlappedRange), "DetachDetours DetourDetach SetFileIoOverlappedRange Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFilePointer, Functions::FileApi::Overrides::SetFilePointer), "DetachDetours DetourDetach SetFilePointer Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFilePointerEx, Functions::FileApi::Overrides::SetFilePointerEx), "DetachDetours DetourDetach SetFilePointerEx Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFileTime, Functions::FileApi::Overrides::SetFileTime), "DetachDetours DetourDetach SetFileTime Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::SetFileValidData, Functions::FileApi::Overrides::SetFileValidData), "DetachDetours DetourDetach SetFileValidData Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::UnlockFile, Functions::FileApi::Overrides::UnlockFile), "DetachDetours DetourDetach UnlockFile Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::UnlockFileEx, Functions::FileApi::Overrides::UnlockFileEx), "DetachDetours DetourDetach UnlockFileEx Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::WriteFile, Functions::FileApi::Overrides::WriteFile), "DetachDetours DetourDetach WriteFile Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::WriteFileEx, Functions::FileApi::Overrides::WriteFileEx), "DetachDetours DetourDetach WriteFileEx Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::FileApi::Cache::WriteFileGather, Functions::FileApi::Overrides::WriteFileGather), "DetachDetours DetourDetach WriteFileGather Failed");

	// LibLoaderApi
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::LibLoaderApi::Cache::LoadLibraryA, Functions::LibLoaderApi::Overrides::LoadLibraryA), "DetachDetours DetourDetach LoadLibraryA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::LibLoaderApi::Cache::LoadLibraryW, Functions::LibLoaderApi::Overrides::LoadLibraryW), "DetachDetours DetourDetach LoadLibraryW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::LibLoaderApi::Cache::LoadLibraryExA, Functions::LibLoaderApi::Overrides::LoadLibraryExA), "DetachDetours DetourDetach LoadLibraryExA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::LibLoaderApi::Cache::LoadLibraryExW, Functions::LibLoaderApi::Overrides::LoadLibraryExW), "DetachDetours DetourDetach LoadLibraryExW Failed");

	// ProcessEnv
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::ProcessEnv::Cache::SearchPathA, Functions::ProcessEnv::Overrides::SearchPathA), "DetachDetours DetourDetach SearchPathA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::ProcessEnv::Cache::SearchPathW, Functions::ProcessEnv::Overrides::SearchPathW), "DetachDetours DetourDetach SearchPathW Failed");

	// ProcessThreadsApi
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::ProcessThreadsApi::Cache::CreateProcessA, Functions::ProcessThreadsApi::Overrides::CreateProcessA), "DetachDetours DetourDetach CreateProcessA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::ProcessThreadsApi::Cache::CreateProcessW, Functions::ProcessThreadsApi::Overrides::CreateProcessW), "DetachDetours DetourDetach CreateProcessW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::ProcessThreadsApi::Cache::CreateProcessAsUserA, Functions::ProcessThreadsApi::Overrides::CreateProcessAsUserA), "DetachDetours DetourDetach CreateProcessAsUserA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::ProcessThreadsApi::Cache::CreateProcessAsUserW, Functions::ProcessThreadsApi::Overrides::CreateProcessAsUserW), "DetachDetours DetourDetach CreateProcessAsUserW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::ProcessThreadsApi::Cache::ExitProcess, Functions::ProcessThreadsApi::Overrides::ExitProcess), "DetachDetours DetourDetach ExitProcess Failed");

	// UndocumentedApi
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::UndocumentedApi::Cache::EntryPoint, Functions::UndocumentedApi::Overrides::EntryPoint), "DetachDetours DetourDetach EntryPoint Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::UndocumentedApi::Cache::PrivCopyFileExA, Functions::UndocumentedApi::Overrides::PrivCopyFileExA), "DetachDetours DetourDetach PrivCopyFileExA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::UndocumentedApi::Cache::PrivCopyFileExW, Functions::UndocumentedApi::Overrides::PrivCopyFileExW), "DetachDetours DetourDetach PrivCopyFileExW Failed");

	// WinBase
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CopyFileA, Functions::WinBase::Overrides::CopyFileA), "DetachDetours DetourDetach CopyFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CopyFileW, Functions::WinBase::Overrides::CopyFileW), "DetachDetours DetourDetach CopyFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CopyFile2, Functions::WinBase::Overrides::CopyFile2), "DetachDetours DetourDetach CopyFile2 Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CopyFileExA, Functions::WinBase::Overrides::CopyFileExA), "DetachDetours DetourDetach CopyFileExA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CopyFileExW, Functions::WinBase::Overrides::CopyFileExW), "DetachDetours DetourDetach CopyFileExW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CopyFileTransactedA, Functions::WinBase::Overrides::CopyFileTransactedA), "DetachDetours DetourDetach CopyFileTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CopyFileTransactedW, Functions::WinBase::Overrides::CopyFileTransactedW), "DetachDetours DetourDetach CopyFileTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateDirectoryExA, Functions::WinBase::Overrides::CreateDirectoryExA), "DetachDetours DetourDetach CreateDirectoryExA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateDirectoryExW, Functions::WinBase::Overrides::CreateDirectoryExW), "DetachDetours DetourDetach CreateDirectoryExW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateDirectoryTransactedA, Functions::WinBase::Overrides::CreateDirectoryTransactedA), "DetachDetours DetourDetach CreateDirectoryTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateDirectoryTransactedW, Functions::WinBase::Overrides::CreateDirectoryTransactedW), "DetachDetours DetourDetach CreateDirectoryTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateFileTransactedA, Functions::WinBase::Overrides::CreateFileTransactedA), "DetachDetours DetourDetach CreateFileTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateFileTransactedW, Functions::WinBase::Overrides::CreateFileTransactedW), "DetachDetours DetourDetach CreateFileTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateHardLinkA, Functions::WinBase::Overrides::CreateHardLinkA), "DetachDetours DetourDetach CreateHardLinkA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateHardLinkW, Functions::WinBase::Overrides::CreateHardLinkW), "DetachDetours DetourDetach CreateHardLinkW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateHardLinkTransactedA, Functions::WinBase::Overrides::CreateHardLinkTransactedA), "DetachDetours DetourDetach CreateHardLinkTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateHardLinkTransactedW, Functions::WinBase::Overrides::CreateHardLinkTransactedW), "DetachDetours DetourDetach CreateHardLinkTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateProcessWithLogonW, Functions::WinBase::Overrides::CreateProcessWithLogonW), "DetachDetours DetourDetach CreateProcessWithLogonW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateProcessWithTokenW, Functions::WinBase::Overrides::CreateProcessWithTokenW), "DetachDetours DetourDetach CreateProcessWithTokenW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateSymbolicLinkA, Functions::WinBase::Overrides::CreateSymbolicLinkA), "DetachDetours DetourDetach CreateSymbolicLinkA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateSymbolicLinkW, Functions::WinBase::Overrides::CreateSymbolicLinkW), "DetachDetours DetourDetach CreateSymbolicLinkW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateSymbolicLinkTransactedA, Functions::WinBase::Overrides::CreateSymbolicLinkTransactedA), "DetachDetours DetourDetach CreateSymbolicLinkTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::CreateSymbolicLinkTransactedW, Functions::WinBase::Overrides::CreateSymbolicLinkTransactedW), "DetachDetours DetourDetach CreateSymbolicLinkTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::DecryptFileA, Functions::WinBase::Overrides::DecryptFileA), "DetachDetours DetourDetach DecryptFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::DecryptFileW, Functions::WinBase::Overrides::DecryptFileW), "DetachDetours DetourDetach DecryptFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::DeleteFileTransactedA, Functions::WinBase::Overrides::DeleteFileTransactedA), "DetachDetours DetourDetach DeleteFileTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::DeleteFileTransactedW, Functions::WinBase::Overrides::DeleteFileTransactedW), "DetachDetours DetourDetach DeleteFileTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::EncryptFileA, Functions::WinBase::Overrides::EncryptFileA), "DetachDetours DetourDetach EncryptFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::EncryptFileW, Functions::WinBase::Overrides::EncryptFileW), "DetachDetours DetourDetach EncryptFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::FileEncryptionStatusA, Functions::WinBase::Overrides::FileEncryptionStatusA), "DetachDetours DetourDetach FileEncryptionStatusA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::FileEncryptionStatusW, Functions::WinBase::Overrides::FileEncryptionStatusW), "DetachDetours DetourDetach FileEncryptionStatusW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::FindFirstFileNameTransactedW, Functions::WinBase::Overrides::FindFirstFileNameTransactedW), "DetachDetours DetourDetach FindFirstFileNameTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::FindFirstFileTransactedA, Functions::WinBase::Overrides::FindFirstFileTransactedA), "DetachDetours DetourDetach FindFirstFileTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::FindFirstFileTransactedW, Functions::WinBase::Overrides::FindFirstFileTransactedW), "DetachDetours DetourDetach FindFirstFileTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::FindFirstStreamTransactedW, Functions::WinBase::Overrides::FindFirstStreamTransactedW), "DetachDetours DetourDetach FindFirstStreamTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetBinaryTypeA, Functions::WinBase::Overrides::GetBinaryTypeA), "DetachDetours DetourDetach GetBinaryTypeA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetBinaryTypeW, Functions::WinBase::Overrides::GetBinaryTypeW), "DetachDetours DetourDetach GetBinaryTypeW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetCompressedFileSizeTransactedA, Functions::WinBase::Overrides::GetCompressedFileSizeTransactedA), "DetachDetours DetourDetach GetCompressedFileSizeTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetCompressedFileSizeTransactedW, Functions::WinBase::Overrides::GetCompressedFileSizeTransactedW), "DetachDetours DetourDetach GetCompressedFileSizeTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetDllDirectoryA, Functions::WinBase::Overrides::GetDllDirectoryA), "DetachDetours DetourDetach GetDllDirectoryA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetDllDirectoryW, Functions::WinBase::Overrides::GetDllDirectoryW), "DetachDetours DetourDetach GetDllDirectoryW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetFileAttributesTransactedA, Functions::WinBase::Overrides::GetFileAttributesTransactedA), "DetachDetours DetourDetach GetFileAttributesTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetFileAttributesTransactedW, Functions::WinBase::Overrides::GetFileAttributesTransactedW), "DetachDetours DetourDetach GetFileAttributesTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetFileBandwidthReservation, Functions::WinBase::Overrides::GetFileBandwidthReservation), "DetachDetours DetourDetach GetFileBandwidthReservation Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetFileInformationByHandleEx, Functions::WinBase::Overrides::GetFileInformationByHandleEx), "DetachDetours DetourDetach GetFileInformationByHandleEx Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetFileSecurityA, Functions::WinBase::Overrides::GetFileSecurityA), "DetachDetours DetourDetach GetFileSecurityA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetFullPathNameTransactedA, Functions::WinBase::Overrides::GetFullPathNameTransactedA), "DetachDetours DetourDetach GetFullPathNameTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetFullPathNameTransactedW, Functions::WinBase::Overrides::GetFullPathNameTransactedW), "DetachDetours DetourDetach GetFullPathNameTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetLongPathNameTransactedA, Functions::WinBase::Overrides::GetLongPathNameTransactedA), "DetachDetours DetourDetach GetLongPathNameTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetLongPathNameTransactedW, Functions::WinBase::Overrides::GetLongPathNameTransactedW), "DetachDetours DetourDetach GetLongPathNameTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetQueuedCompletionStatus, Functions::WinBase::Overrides::GetQueuedCompletionStatus), "DetachDetours DetourDetach GetQueuedCompletionStatus Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetQueuedCompletionStatusEx, Functions::WinBase::Overrides::GetQueuedCompletionStatusEx), "DetachDetours DetourDetach GetQueuedCompletionStatusEx Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::GetShortPathNameA, Functions::WinBase::Overrides::GetShortPathNameA), "DetachDetours DetourDetach GetShortPathNameA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::LoadModule, Functions::WinBase::Overrides::LoadModule), "DetachDetours DetourDetach LoadModule Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::LoadPackagedLibrary, Functions::WinBase::Overrides::LoadPackagedLibrary), "DetachDetours DetourDetach LoadPackagedLibrary Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::MoveFileA, Functions::WinBase::Overrides::MoveFileA), "DetachDetours DetourDetach MoveFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::MoveFileW, Functions::WinBase::Overrides::MoveFileW), "DetachDetours DetourDetach MoveFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::MoveFileExA, Functions::WinBase::Overrides::MoveFileExA), "DetachDetours DetourDetach MoveFileExA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::MoveFileExW, Functions::WinBase::Overrides::MoveFileExW), "DetachDetours DetourDetach MoveFileExW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::MoveFileTransactedA, Functions::WinBase::Overrides::MoveFileTransactedA), "DetachDetours DetourDetach MoveFileTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::MoveFileTransactedW, Functions::WinBase::Overrides::MoveFileTransactedW), "DetachDetours DetourDetach MoveFileTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::MoveFileWithProgressA, Functions::WinBase::Overrides::MoveFileWithProgressA), "DetachDetours DetourDetach MoveFileWithProgressA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::MoveFileWithProgressW, Functions::WinBase::Overrides::MoveFileWithProgressW), "DetachDetours DetourDetach MoveFileWithProgressW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::OpenEncryptedFileRawA, Functions::WinBase::Overrides::OpenEncryptedFileRawA), "DetachDetours DetourDetach OpenEncryptedFileRawA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::OpenEncryptedFileRawW, Functions::WinBase::Overrides::OpenEncryptedFileRawW), "DetachDetours DetourDetach OpenEncryptedFileRawW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::OpenFile, Functions::WinBase::Overrides::OpenFile), "DetachDetours DetourDetach OpenFile Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::OpenFileById, Functions::WinBase::Overrides::OpenFileById), "DetachDetours DetourDetach OpenFileById Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::ReadEncryptedFileRaw, Functions::WinBase::Overrides::ReadEncryptedFileRaw), "DetachDetours DetourDetach ReadEncryptedFileRaw Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::RemoveDirectoryTransactedA, Functions::WinBase::Overrides::RemoveDirectoryTransactedA), "DetachDetours DetourDetach RemoveDirectoryTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::RemoveDirectoryTransactedW, Functions::WinBase::Overrides::RemoveDirectoryTransactedW), "DetachDetours DetourDetach RemoveDirectoryTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::ReOpenFile, Functions::WinBase::Overrides::ReOpenFile), "DetachDetours DetourDetach ReOpenFile Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::ReplaceFileA, Functions::WinBase::Overrides::ReplaceFileA), "DetachDetours DetourDetach ReplaceFileA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::ReplaceFileW, Functions::WinBase::Overrides::ReplaceFileW), "DetachDetours DetourDetach ReplaceFileW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetCurrentDirectoryA, Functions::WinBase::Overrides::SetCurrentDirectoryA), "DetachDetours DetourDetach SetCurrentDirectoryA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetCurrentDirectoryW, Functions::WinBase::Overrides::SetCurrentDirectoryW), "DetachDetours DetourDetach SetCurrentDirectoryW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetDllDirectoryA, Functions::WinBase::Overrides::SetDllDirectoryA), "DetachDetours DetourDetach SetDllDirectoryA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetDllDirectoryW, Functions::WinBase::Overrides::SetDllDirectoryW), "DetachDetours DetourDetach SetDllDirectoryW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetFileAttributesTransactedA, Functions::WinBase::Overrides::SetFileAttributesTransactedA), "DetachDetours DetourDetach SetFileAttributesTransactedA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetFileAttributesTransactedW, Functions::WinBase::Overrides::SetFileAttributesTransactedW), "DetachDetours DetourDetach SetFileAttributesTransactedW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetFileBandwidthReservation, Functions::WinBase::Overrides::SetFileBandwidthReservation), "DetachDetours DetourDetach SetFileBandwidthReservation Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetFileCompletionNotificationModes, Functions::WinBase::Overrides::SetFileCompletionNotificationModes), "DetachDetours DetourDetach SetFileCompletionNotificationModes Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetFileSecurityA, Functions::WinBase::Overrides::SetFileSecurityA), "DetachDetours DetourDetach SetFileSecurityA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetFileShortNameA, Functions::WinBase::Overrides::SetFileShortNameA), "DetachDetours DetourDetach SetFileShortNameA Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetFileShortNameW, Functions::WinBase::Overrides::SetFileShortNameW), "DetachDetours DetourDetach SetFileShortNameW Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::SetSearchPathMode, Functions::WinBase::Overrides::SetSearchPathMode), "DetachDetours DetourDetach SetSearchPathMode Failed");
	ThrowIfFailed(DetourDetach(&(PVOID&)Functions::WinBase::Cache::WriteEncryptedFileRaw, Functions::WinBase::Overrides::WriteEncryptedFileRaw), "DetachDetours DetourDetach WriteEncryptedFileRaw Failed");

	ThrowIfFailed(DetourTransactionCommit(), "DetachDetours DetourTransactionCommit Failed");
}

bool ThreadAttach(HMODULE /*hDll*/)
{
	return true;
}

bool ThreadDetach(HMODULE /*hDll*/)
{
	return true;
}

std::vector<std::string> ExtractStringList(const char* rawValues, uint64_t length)
{
	auto result = std::vector<std::string>();
	auto remainingContent = length;
	auto valueBuffer = rawValues;

	// Keep pulling off strings until we reach the end
	while (remainingContent > 0)
	{
		auto value = std::string(valueBuffer);
		valueBuffer += value.length() + 1;
		remainingContent -= value.length() + 1;

		result.push_back(std::move(value));
	}

	return result;
}

bool ProcessAttach(HMODULE hDll)
{
	InitializeCriticalSection(&s_csChildPayload);

	s_hInst = hDll;
	s_hKernel32 = nullptr;

	PBYTE xCreate = (PBYTE)DetourCodeFromPointer((PVOID)Functions::ProcessThreadsApi::Cache::CreateProcessW, nullptr);
	Monitor::ProcessPayload* pPayload = nullptr;

	for (HMODULE hMod = nullptr; (hMod = DetourEnumerateModules(hMod)) != nullptr;)
	{
		ULONG cbData;
		PVOID pvData = DetourFindPayload(hMod, Monitor::ProcessPayloadResourceId, &cbData);

		if (pvData != nullptr && pPayload == nullptr)
		{
			pPayload = (Monitor::ProcessPayload*)pvData;
		}

		ULONG cbMod = DetourGetModuleSize(hMod);

		if (((PBYTE)hMod) < xCreate && ((PBYTE)hMod + cbMod) > xCreate)
		{
			s_hKernel32 = hMod;
		}
	}

	ZeroMemory(&s_Payload, sizeof(s_Payload));

	if (pPayload == nullptr)
	{
		return false;
	}

	CopyMemory(&s_Payload, pPayload, sizeof(s_Payload));

	GetModuleFileNameA(s_hInst, s_szDllPath, ARRAYSIZE(s_szDllPath));

	try
	{
		auto traceProcessId = s_Payload.nTraceProcessId;

		// Extract the allowed read/write directories
		auto allowedReadDirectories = ExtractStringList(s_Payload.zReadAccessDirectories, s_Payload.cReadAccessDirectories);
		auto allowedWriteDirectories = ExtractStringList(s_Payload.zWriteAccessDirectories, s_Payload.cWriteAccessDirectories);

		// Initialize the event pipe
		Monitor::ConnectionManager::Initialize(traceProcessId);
		Monitor::FileSystemAccessSandbox::Initialize(
			std::move(allowedReadDirectories),
			std::move(allowedWriteDirectories));

		// Find hidden functions.
		Functions::UndocumentedApi::Cache::PrivCopyFileExA =
			(BOOL (WINAPI *)(LPCSTR, LPCSTR, LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD))
			GetProcAddress(s_hKernel32, "PrivCopyFileExW");
		if (Functions::UndocumentedApi::Cache::PrivCopyFileExA == nullptr)
		{
			throw std::runtime_error("Failed to GetProcAddress PrivCopyFileExA");
		}

		// Find hidden functions.
		Functions::UndocumentedApi::Cache::PrivCopyFileExW =
			(BOOL (WINAPI *)(LPCWSTR, LPCWSTR, LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD))
			GetProcAddress(s_hKernel32, "PrivCopyFileExW");
		if (Functions::UndocumentedApi::Cache::PrivCopyFileExW == nullptr)
		{
			throw std::runtime_error("Failed to GetProcAddress PrivCopyFileExW");
		}
		
		AttachDetours();
	}
	catch (const std::exception& ex)
	{
		Monitor::ConnectionManager::WriteError(ex.what());
		exit(-1234);
	}
	catch (...)
	{
		Monitor::ConnectionManager::WriteError("Unknown error attaching detours");
		exit(-1234);
	}

	ThreadAttach(hDll);

	return true;
}

bool ProcessDetach(HMODULE hDll)
{
	ThreadDetach(hDll);

	try
	{
		DetachDetours();
	}
	catch (const std::exception& ex)
	{
		Monitor::ConnectionManager::WriteError(ex.what());
		exit(-1234);
	}
	catch (...)
	{
		Monitor::ConnectionManager::WriteError("Unknown error detaching detours");
		exit(-1234);
	}

	Monitor::ConnectionManager::Shutdown();

	return true;
}

bool APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
	(void)hModule;
	(void)lpReserved;

#ifdef ENABLE_MONITOR_DEBUG
	while (!IsDebuggerPresent())
	{
		Sleep(0);
	}
#endif

	if (DetourIsHelperProcess())
	{
		return true;
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DetourRestoreAfterWith();
		Functions::UndocumentedApi::Cache::EntryPoint = (int (WINAPI *)(void))DetourGetEntryPoint(nullptr);
		return ProcessAttach(hModule);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		return ProcessDetach(hModule);
	}
	else if (dwReason == DLL_THREAD_ATTACH)
	{
		return ThreadAttach(hModule);
	}
	else if (dwReason == DLL_THREAD_DETACH)
	{
		return ThreadDetach(hModule);
	}

	return true;
}