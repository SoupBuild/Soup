#pragma once

namespace Monitor::Windows
{
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
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::AreFileApisANSI, Functions::Overrides::FileApi::AreFileApisANSI), "AttachDetours DetourAttach AreFileApisANSI Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::CompareFileTime, Functions::Overrides::FileApi::CompareFileTime), "AttachDetours DetourAttach CompareFileTime Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::CreateDirectoryA, Functions::Overrides::FileApi::CreateDirectoryA), "AttachDetours DetourAttach CreateDirectoryA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::CreateDirectoryW, Functions::Overrides::FileApi::CreateDirectoryW), "AttachDetours DetourAttach CreateDirectoryW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::CreateFile2, Functions::Overrides::FileApi::CreateFile2), "AttachDetours DetourAttach CreateFile2 Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::CreateFileA, Functions::Overrides::FileApi::CreateFileA), "AttachDetours DetourAttach CreateFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::CreateFileW, Functions::Overrides::FileApi::CreateFileW), "AttachDetours DetourAttach CreateFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::DefineDosDeviceW, Functions::Overrides::FileApi::DefineDosDeviceW), "AttachDetours DetourAttach DefineDosDeviceW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::DeleteFileA, Functions::Overrides::FileApi::DeleteFileA), "AttachDetours DetourAttach DeleteFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::DeleteFileW, Functions::Overrides::FileApi::DeleteFileW), "AttachDetours DetourAttach DeleteFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::DeleteVolumeMountPointW, Functions::Overrides::FileApi::DeleteVolumeMountPointW), "AttachDetours DetourAttach DeleteVolumeMountPointW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FileTimeToLocalFileTime, Functions::Overrides::FileApi::FileTimeToLocalFileTime), "AttachDetours DetourAttach FileTimeToLocalFileTime Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindClose, Functions::Overrides::FileApi::FindClose), "AttachDetours DetourAttach FindClose Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindCloseChangeNotification, Functions::Overrides::FileApi::FindCloseChangeNotification), "AttachDetours DetourAttach FindCloseChangeNotification Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstChangeNotificationA, Functions::Overrides::FileApi::FindFirstChangeNotificationA), "AttachDetours DetourAttach FindFirstChangeNotificationA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstChangeNotificationW, Functions::Overrides::FileApi::FindFirstChangeNotificationW), "AttachDetours DetourAttach FindFirstChangeNotificationW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileA, Functions::Overrides::FileApi::FindFirstFileA), "AttachDetours DetourAttach FindFirstFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileW, Functions::Overrides::FileApi::FindFirstFileW), "AttachDetours DetourAttach FindFirstFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileExA, Functions::Overrides::FileApi::FindFirstFileExA), "AttachDetours DetourAttach FindFirstFileExA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileExW, Functions::Overrides::FileApi::FindFirstFileExW), "AttachDetours DetourAttach FindFirstFileExW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileNameW, Functions::Overrides::FileApi::FindFirstFileNameW), "AttachDetours DetourAttach FindFirstFileNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstStreamW, Functions::Overrides::FileApi::FindFirstStreamW), "AttachDetours DetourAttach FindFirstStreamW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindFirstVolumeW, Functions::Overrides::FileApi::FindFirstVolumeW), "AttachDetours DetourAttach FindFirstVolumeW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindNextChangeNotification, Functions::Overrides::FileApi::FindNextChangeNotification), "AttachDetours DetourAttach FindNextChangeNotification Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindNextFileA, Functions::Overrides::FileApi::FindNextFileA), "AttachDetours DetourAttach FindNextFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindNextFileW, Functions::Overrides::FileApi::FindNextFileW), "AttachDetours DetourAttach FindNextFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindNextFileNameW, Functions::Overrides::FileApi::FindNextFileNameW), "AttachDetours DetourAttach FindNextFileNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindNextStreamW, Functions::Overrides::FileApi::FindNextStreamW), "AttachDetours DetourAttach FindNextStreamW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindNextVolumeW, Functions::Overrides::FileApi::FindNextVolumeW), "AttachDetours DetourAttach FindNextVolumeW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FindVolumeClose, Functions::Overrides::FileApi::FindVolumeClose), "AttachDetours DetourAttach FindVolumeClose Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::FlushFileBuffers, Functions::Overrides::FileApi::FlushFileBuffers), "AttachDetours DetourAttach FlushFileBuffers Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetCompressedFileSizeA, Functions::Overrides::FileApi::GetCompressedFileSizeA), "AttachDetours DetourAttach GetCompressedFileSizeA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetCompressedFileSizeW, Functions::Overrides::FileApi::GetCompressedFileSizeW), "AttachDetours DetourAttach GetCompressedFileSizeW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetDiskFreeSpaceA, Functions::Overrides::FileApi::GetDiskFreeSpaceA), "AttachDetours DetourAttach GetDiskFreeSpaceA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetDiskFreeSpaceW, Functions::Overrides::FileApi::GetDiskFreeSpaceW), "AttachDetours DetourAttach GetDiskFreeSpaceW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetDiskFreeSpaceExA, Functions::Overrides::FileApi::GetDiskFreeSpaceExA), "AttachDetours DetourAttach GetDiskFreeSpaceExA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetDiskFreeSpaceExW, Functions::Overrides::FileApi::GetDiskFreeSpaceExW), "AttachDetours DetourAttach GetDiskFreeSpaceExW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetDriveTypeA, Functions::Overrides::FileApi::GetDriveTypeA), "AttachDetours DetourAttach GetDriveTypeA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetDriveTypeW, Functions::Overrides::FileApi::GetDriveTypeW), "AttachDetours DetourAttach GetDriveTypeW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileAttributesA, Functions::Overrides::FileApi::GetFileAttributesA), "AttachDetours DetourAttach GetFileAttributesA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileAttributesW, Functions::Overrides::FileApi::GetFileAttributesW), "AttachDetours DetourAttach GetFileAttributesW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileAttributesExA, Functions::Overrides::FileApi::GetFileAttributesExA), "AttachDetours DetourAttach GetFileAttributesExA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileAttributesExW, Functions::Overrides::FileApi::GetFileAttributesExW), "AttachDetours DetourAttach GetFileAttributesExW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileInformationByHandle, Functions::Overrides::FileApi::GetFileInformationByHandle), "AttachDetours DetourAttach GetFileInformationByHandle Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileSize, Functions::Overrides::FileApi::GetFileSize), "AttachDetours DetourAttach GetFileSize Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileSizeEx, Functions::Overrides::FileApi::GetFileSizeEx), "AttachDetours DetourAttach GetFileSizeEx Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileTime, Functions::Overrides::FileApi::GetFileTime), "AttachDetours DetourAttach GetFileTime Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFileType, Functions::Overrides::FileApi::GetFileType), "AttachDetours DetourAttach GetFileType Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFinalPathNameByHandleA, Functions::Overrides::FileApi::GetFinalPathNameByHandleA), "AttachDetours DetourAttach GetFinalPathNameByHandleA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFinalPathNameByHandleW, Functions::Overrides::FileApi::GetFinalPathNameByHandleW), "AttachDetours DetourAttach GetFinalPathNameByHandleW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFullPathNameA, Functions::Overrides::FileApi::GetFullPathNameA), "AttachDetours DetourAttach GetFullPathNameA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetFullPathNameW, Functions::Overrides::FileApi::GetFullPathNameW), "AttachDetours DetourAttach GetFullPathNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetLogicalDrives, Functions::Overrides::FileApi::GetLogicalDrives), "AttachDetours DetourAttach GetLogicalDrives Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetLogicalDriveStringsW, Functions::Overrides::FileApi::GetLogicalDriveStringsW), "AttachDetours DetourAttach GetLogicalDriveStringsW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetLongPathNameA, Functions::Overrides::FileApi::GetLongPathNameA), "AttachDetours DetourAttach GetLongPathNameA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetLongPathNameW, Functions::Overrides::FileApi::GetLongPathNameW), "AttachDetours DetourAttach GetLongPathNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetShortPathNameW, Functions::Overrides::FileApi::GetShortPathNameW), "AttachDetours DetourAttach GetShortPathNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetTempFileNameA, Functions::Overrides::FileApi::GetTempFileNameA), "AttachDetours DetourAttach GetTempFileNameA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetTempFileNameW, Functions::Overrides::FileApi::GetTempFileNameW), "AttachDetours DetourAttach GetTempFileNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetTempPathA, Functions::Overrides::FileApi::GetTempPathA), "AttachDetours DetourAttach GetTempPathA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetTempPathW, Functions::Overrides::FileApi::GetTempPathW), "AttachDetours DetourAttach GetTempPathW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetVolumeInformationA, Functions::Overrides::FileApi::GetVolumeInformationA), "AttachDetours DetourAttach GetVolumeInformationA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetVolumeInformationW, Functions::Overrides::FileApi::GetVolumeInformationW), "AttachDetours DetourAttach GetVolumeInformationW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetVolumeInformationByHandleW, Functions::Overrides::FileApi::GetVolumeInformationByHandleW), "AttachDetours DetourAttach GetVolumeInformationByHandleW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetVolumeNameForVolumeMountPointW, Functions::Overrides::FileApi::GetVolumeNameForVolumeMountPointW), "AttachDetours DetourAttach GetVolumeNameForVolumeMountPointW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetVolumePathNamesForVolumeNameW, Functions::Overrides::FileApi::GetVolumePathNamesForVolumeNameW), "AttachDetours DetourAttach GetVolumePathNamesForVolumeNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::GetVolumePathNameW, Functions::Overrides::FileApi::GetVolumePathNameW), "AttachDetours DetourAttach GetVolumePathNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::LocalFileTimeToFileTime, Functions::Overrides::FileApi::LocalFileTimeToFileTime), "AttachDetours DetourAttach LocalFileTimeToFileTime Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::LockFile, Functions::Overrides::FileApi::LockFile), "AttachDetours DetourAttach LockFile Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::LockFileEx, Functions::Overrides::FileApi::LockFileEx), "AttachDetours DetourAttach LockFileEx Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::QueryDosDeviceW, Functions::Overrides::FileApi::QueryDosDeviceW), "AttachDetours DetourAttach QueryDosDeviceW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::ReadFile, Functions::Overrides::FileApi::ReadFile), "AttachDetours DetourAttach ReadFile Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::ReadFileEx, Functions::Overrides::FileApi::ReadFileEx), "AttachDetours DetourAttach ReadFileEx Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::ReadFileScatter, Functions::Overrides::FileApi::ReadFileScatter), "AttachDetours DetourAttach ReadFileScatter Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::RemoveDirectoryA, Functions::Overrides::FileApi::RemoveDirectoryA), "AttachDetours DetourAttach RemoveDirectoryA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::RemoveDirectoryW, Functions::Overrides::FileApi::RemoveDirectoryW), "AttachDetours DetourAttach RemoveDirectoryW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetEndOfFile, Functions::Overrides::FileApi::SetEndOfFile), "AttachDetours DetourAttach SetEndOfFile Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFileApisToANSI, Functions::Overrides::FileApi::SetFileApisToANSI), "AttachDetours DetourAttach SetFileApisToANSI Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFileApisToOEM, Functions::Overrides::FileApi::SetFileApisToOEM), "AttachDetours DetourAttach SetFileApisToOEM Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFileAttributesA, Functions::Overrides::FileApi::SetFileAttributesA), "AttachDetours DetourAttach SetFileAttributesA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFileAttributesW, Functions::Overrides::FileApi::SetFileAttributesW), "AttachDetours DetourAttach SetFileAttributesW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFileInformationByHandle, Functions::Overrides::FileApi::SetFileInformationByHandle), "AttachDetours DetourAttach SetFileInformationByHandle Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFileIoOverlappedRange, Functions::Overrides::FileApi::SetFileIoOverlappedRange), "AttachDetours DetourAttach SetFileIoOverlappedRange Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFilePointer, Functions::Overrides::FileApi::SetFilePointer), "AttachDetours DetourAttach SetFilePointer Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFilePointerEx, Functions::Overrides::FileApi::SetFilePointerEx), "AttachDetours DetourAttach SetFilePointerEx Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFileTime, Functions::Overrides::FileApi::SetFileTime), "AttachDetours DetourAttach SetFileTime Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::SetFileValidData, Functions::Overrides::FileApi::SetFileValidData), "AttachDetours DetourAttach SetFileValidData Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::UnlockFile, Functions::Overrides::FileApi::UnlockFile), "AttachDetours DetourAttach UnlockFile Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::UnlockFileEx, Functions::Overrides::FileApi::UnlockFileEx), "AttachDetours DetourAttach UnlockFileEx Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::WriteFile, Functions::Overrides::FileApi::WriteFile), "AttachDetours DetourAttach WriteFile Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::WriteFileEx, Functions::Overrides::FileApi::WriteFileEx), "AttachDetours DetourAttach WriteFileEx Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::FileApi::WriteFileGather, Functions::Overrides::FileApi::WriteFileGather), "AttachDetours DetourAttach WriteFileGather Failed");

		// LibLoaderApi
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::LibLoaderApi::LoadLibraryA, Functions::Overrides::LibLoaderApi::LoadLibraryA), "AttachDetours DetourAttach LoadLibraryA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::LibLoaderApi::LoadLibraryW, Functions::Overrides::LibLoaderApi::LoadLibraryW), "AttachDetours DetourAttach LoadLibraryW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::LibLoaderApi::LoadLibraryExA, Functions::Overrides::LibLoaderApi::LoadLibraryExA), "AttachDetours DetourAttach LoadLibraryExA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::LibLoaderApi::LoadLibraryExW, Functions::Overrides::LibLoaderApi::LoadLibraryExW), "AttachDetours DetourAttach LoadLibraryExW Failed");

		// ProcessEnv
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ProcessEnv::SearchPathA, Functions::Overrides::ProcessEnv::SearchPathA), "AttachDetours DetourAttach SearchPathA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ProcessEnv::SearchPathW, Functions::Overrides::ProcessEnv::SearchPathW), "AttachDetours DetourAttach SearchPathW Failed");

		// ProcessThreadsApi
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ProcessThreadsApi::CreateProcessA, Functions::Overrides::ProcessThreadsApi::CreateProcessA), "AttachDetours DetourAttach CreateProcessA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ProcessThreadsApi::CreateProcessW, Functions::Overrides::ProcessThreadsApi::CreateProcessW), "AttachDetours DetourAttach CreateProcessW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ProcessThreadsApi::CreateProcessAsUserA, Functions::Overrides::ProcessThreadsApi::CreateProcessAsUserA), "AttachDetours DetourAttach CreateProcessAsUserA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ProcessThreadsApi::CreateProcessAsUserW, Functions::Overrides::ProcessThreadsApi::CreateProcessAsUserW), "AttachDetours DetourAttach CreateProcessAsUserW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ProcessThreadsApi::ExitProcess, Functions::Overrides::ProcessThreadsApi::ExitProcess), "AttachDetours DetourAttach ExitProcess Failed");

		// UndocumentedApi
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::UndocumentedApi::EntryPoint, Functions::Overrides::UndocumentedApi::EntryPoint), "AttachDetours DetourAttach EntryPoint Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::UndocumentedApi::PrivCopyFileExA, Functions::Overrides::UndocumentedApi::PrivCopyFileExA), "AttachDetours DetourAttach PrivCopyFileExA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::UndocumentedApi::PrivCopyFileExW, Functions::Overrides::UndocumentedApi::PrivCopyFileExW), "AttachDetours DetourAttach PrivCopyFileExW Failed");

		// WinBase
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CopyFileA, Functions::Overrides::WinBase::CopyFileA), "AttachDetours DetourAttach CopyFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CopyFileW, Functions::Overrides::WinBase::CopyFileW), "AttachDetours DetourAttach CopyFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CopyFile2, Functions::Overrides::WinBase::CopyFile2), "AttachDetours DetourAttach CopyFile2 Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CopyFileExA, Functions::Overrides::WinBase::CopyFileExA), "AttachDetours DetourAttach CopyFileExA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CopyFileExW, Functions::Overrides::WinBase::CopyFileExW), "AttachDetours DetourAttach CopyFileExW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CopyFileTransactedA, Functions::Overrides::WinBase::CopyFileTransactedA), "AttachDetours DetourAttach CopyFileTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CopyFileTransactedW, Functions::Overrides::WinBase::CopyFileTransactedW), "AttachDetours DetourAttach CopyFileTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateDirectoryExA, Functions::Overrides::WinBase::CreateDirectoryExA), "AttachDetours DetourAttach CreateDirectoryExA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateDirectoryExW, Functions::Overrides::WinBase::CreateDirectoryExW), "AttachDetours DetourAttach CreateDirectoryExW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateDirectoryTransactedA, Functions::Overrides::WinBase::CreateDirectoryTransactedA), "AttachDetours DetourAttach CreateDirectoryTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateDirectoryTransactedW, Functions::Overrides::WinBase::CreateDirectoryTransactedW), "AttachDetours DetourAttach CreateDirectoryTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateFileTransactedA, Functions::Overrides::WinBase::CreateFileTransactedA), "AttachDetours DetourAttach CreateFileTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateFileTransactedW, Functions::Overrides::WinBase::CreateFileTransactedW), "AttachDetours DetourAttach CreateFileTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateHardLinkA, Functions::Overrides::WinBase::CreateHardLinkA), "AttachDetours DetourAttach CreateHardLinkA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateHardLinkW, Functions::Overrides::WinBase::CreateHardLinkW), "AttachDetours DetourAttach CreateHardLinkW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateHardLinkTransactedA, Functions::Overrides::WinBase::CreateHardLinkTransactedA), "AttachDetours DetourAttach CreateHardLinkTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateHardLinkTransactedW, Functions::Overrides::WinBase::CreateHardLinkTransactedW), "AttachDetours DetourAttach CreateHardLinkTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateProcessWithLogonW, Functions::Overrides::WinBase::CreateProcessWithLogonW), "AttachDetours DetourAttach CreateProcessWithLogonW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateProcessWithTokenW, Functions::Overrides::WinBase::CreateProcessWithTokenW), "AttachDetours DetourAttach CreateProcessWithTokenW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateSymbolicLinkA, Functions::Overrides::WinBase::CreateSymbolicLinkA), "AttachDetours DetourAttach CreateSymbolicLinkA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateSymbolicLinkW, Functions::Overrides::WinBase::CreateSymbolicLinkW), "AttachDetours DetourAttach CreateSymbolicLinkW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateSymbolicLinkTransactedA, Functions::Overrides::WinBase::CreateSymbolicLinkTransactedA), "AttachDetours DetourAttach CreateSymbolicLinkTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::CreateSymbolicLinkTransactedW, Functions::Overrides::WinBase::CreateSymbolicLinkTransactedW), "AttachDetours DetourAttach CreateSymbolicLinkTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::DecryptFileA, Functions::Overrides::WinBase::DecryptFileA), "AttachDetours DetourAttach DecryptFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::DecryptFileW, Functions::Overrides::WinBase::DecryptFileW), "AttachDetours DetourAttach DecryptFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::DeleteFileTransactedA, Functions::Overrides::WinBase::DeleteFileTransactedA), "AttachDetours DetourAttach DeleteFileTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::DeleteFileTransactedW, Functions::Overrides::WinBase::DeleteFileTransactedW), "AttachDetours DetourAttach DeleteFileTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::EncryptFileA, Functions::Overrides::WinBase::EncryptFileA), "AttachDetours DetourAttach EncryptFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::EncryptFileW, Functions::Overrides::WinBase::EncryptFileW), "AttachDetours DetourAttach EncryptFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::FileEncryptionStatusA, Functions::Overrides::WinBase::FileEncryptionStatusA), "AttachDetours DetourAttach FileEncryptionStatusA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::FileEncryptionStatusW, Functions::Overrides::WinBase::FileEncryptionStatusW), "AttachDetours DetourAttach FileEncryptionStatusW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::FindFirstFileNameTransactedW, Functions::Overrides::WinBase::FindFirstFileNameTransactedW), "AttachDetours DetourAttach FindFirstFileNameTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::FindFirstFileTransactedA, Functions::Overrides::WinBase::FindFirstFileTransactedA), "AttachDetours DetourAttach FindFirstFileTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::FindFirstFileTransactedW, Functions::Overrides::WinBase::FindFirstFileTransactedW), "AttachDetours DetourAttach FindFirstFileTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::FindFirstStreamTransactedW, Functions::Overrides::WinBase::FindFirstStreamTransactedW), "AttachDetours DetourAttach FindFirstStreamTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetBinaryTypeA, Functions::Overrides::WinBase::GetBinaryTypeA), "AttachDetours DetourAttach GetBinaryTypeA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetBinaryTypeW, Functions::Overrides::WinBase::GetBinaryTypeW), "AttachDetours DetourAttach GetBinaryTypeW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetCompressedFileSizeTransactedA, Functions::Overrides::WinBase::GetCompressedFileSizeTransactedA), "AttachDetours DetourAttach GetCompressedFileSizeTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetCompressedFileSizeTransactedW, Functions::Overrides::WinBase::GetCompressedFileSizeTransactedW), "AttachDetours DetourAttach GetCompressedFileSizeTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetDllDirectoryA, Functions::Overrides::WinBase::GetDllDirectoryA), "AttachDetours DetourAttach GetDllDirectoryA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetDllDirectoryW, Functions::Overrides::WinBase::GetDllDirectoryW), "AttachDetours DetourAttach GetDllDirectoryW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetEnvironmentVariableA, Functions::Overrides::WinBase::GetEnvironmentVariableA), "AttachDetours DetourAttach GetEnvironmentVariableA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetEnvironmentVariableW, Functions::Overrides::WinBase::GetEnvironmentVariableW), "AttachDetours DetourAttach GetEnvironmentVariableW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetFileAttributesTransactedA, Functions::Overrides::WinBase::GetFileAttributesTransactedA), "AttachDetours DetourAttach GetFileAttributesTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetFileAttributesTransactedW, Functions::Overrides::WinBase::GetFileAttributesTransactedW), "AttachDetours DetourAttach GetFileAttributesTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetFileBandwidthReservation, Functions::Overrides::WinBase::GetFileBandwidthReservation), "AttachDetours DetourAttach GetFileBandwidthReservation Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetFileInformationByHandleEx, Functions::Overrides::WinBase::GetFileInformationByHandleEx), "AttachDetours DetourAttach GetFileInformationByHandleEx Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetFileSecurityA, Functions::Overrides::WinBase::GetFileSecurityA), "AttachDetours DetourAttach GetFileSecurityA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetFullPathNameTransactedA, Functions::Overrides::WinBase::GetFullPathNameTransactedA), "AttachDetours DetourAttach GetFullPathNameTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetFullPathNameTransactedW, Functions::Overrides::WinBase::GetFullPathNameTransactedW), "AttachDetours DetourAttach GetFullPathNameTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetLongPathNameTransactedA, Functions::Overrides::WinBase::GetLongPathNameTransactedA), "AttachDetours DetourAttach GetLongPathNameTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetLongPathNameTransactedW, Functions::Overrides::WinBase::GetLongPathNameTransactedW), "AttachDetours DetourAttach GetLongPathNameTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetQueuedCompletionStatus, Functions::Overrides::WinBase::GetQueuedCompletionStatus), "AttachDetours DetourAttach GetQueuedCompletionStatus Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetQueuedCompletionStatusEx, Functions::Overrides::WinBase::GetQueuedCompletionStatusEx), "AttachDetours DetourAttach GetQueuedCompletionStatusEx Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::GetShortPathNameA, Functions::Overrides::WinBase::GetShortPathNameA), "AttachDetours DetourAttach GetShortPathNameA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::LoadModule, Functions::Overrides::WinBase::LoadModule), "AttachDetours DetourAttach LoadModule Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::LoadPackagedLibrary, Functions::Overrides::WinBase::LoadPackagedLibrary), "AttachDetours DetourAttach LoadPackagedLibrary Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::MoveFileA, Functions::Overrides::WinBase::MoveFileA), "AttachDetours DetourAttach MoveFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::MoveFileW, Functions::Overrides::WinBase::MoveFileW), "AttachDetours DetourAttach MoveFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::MoveFileExA, Functions::Overrides::WinBase::MoveFileExA), "AttachDetours DetourAttach MoveFileExA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::MoveFileExW, Functions::Overrides::WinBase::MoveFileExW), "AttachDetours DetourAttach MoveFileExW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::MoveFileTransactedA, Functions::Overrides::WinBase::MoveFileTransactedA), "AttachDetours DetourAttach MoveFileTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::MoveFileTransactedW, Functions::Overrides::WinBase::MoveFileTransactedW), "AttachDetours DetourAttach MoveFileTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::MoveFileWithProgressA, Functions::Overrides::WinBase::MoveFileWithProgressA), "AttachDetours DetourAttach MoveFileWithProgressA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::MoveFileWithProgressW, Functions::Overrides::WinBase::MoveFileWithProgressW), "AttachDetours DetourAttach MoveFileWithProgressW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::OpenEncryptedFileRawA, Functions::Overrides::WinBase::OpenEncryptedFileRawA), "AttachDetours DetourAttach OpenEncryptedFileRawA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::OpenEncryptedFileRawW, Functions::Overrides::WinBase::OpenEncryptedFileRawW), "AttachDetours DetourAttach OpenEncryptedFileRawW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::OpenFile, Functions::Overrides::WinBase::OpenFile), "AttachDetours DetourAttach OpenFile Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::OpenFileById, Functions::Overrides::WinBase::OpenFileById), "AttachDetours DetourAttach OpenFileById Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::ReadEncryptedFileRaw, Functions::Overrides::WinBase::ReadEncryptedFileRaw), "AttachDetours DetourAttach ReadEncryptedFileRaw Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::RemoveDirectoryTransactedA, Functions::Overrides::WinBase::RemoveDirectoryTransactedA), "AttachDetours DetourAttach RemoveDirectoryTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::RemoveDirectoryTransactedW, Functions::Overrides::WinBase::RemoveDirectoryTransactedW), "AttachDetours DetourAttach RemoveDirectoryTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::ReOpenFile, Functions::Overrides::WinBase::ReOpenFile), "AttachDetours DetourAttach ReOpenFile Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::ReplaceFileA, Functions::Overrides::WinBase::ReplaceFileA), "AttachDetours DetourAttach ReplaceFileA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::ReplaceFileW, Functions::Overrides::WinBase::ReplaceFileW), "AttachDetours DetourAttach ReplaceFileW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetCurrentDirectoryA, Functions::Overrides::WinBase::SetCurrentDirectoryA), "AttachDetours DetourAttach SetCurrentDirectoryA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetCurrentDirectoryW, Functions::Overrides::WinBase::SetCurrentDirectoryW), "AttachDetours DetourAttach SetCurrentDirectoryW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetDllDirectoryA, Functions::Overrides::WinBase::SetDllDirectoryA), "AttachDetours DetourAttach SetDllDirectoryA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetDllDirectoryW, Functions::Overrides::WinBase::SetDllDirectoryW), "AttachDetours DetourAttach SetDllDirectoryW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetFileAttributesTransactedA, Functions::Overrides::WinBase::SetFileAttributesTransactedA), "AttachDetours DetourAttach SetFileAttributesTransactedA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetFileAttributesTransactedW, Functions::Overrides::WinBase::SetFileAttributesTransactedW), "AttachDetours DetourAttach SetFileAttributesTransactedW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetFileBandwidthReservation, Functions::Overrides::WinBase::SetFileBandwidthReservation), "AttachDetours DetourAttach SetFileBandwidthReservation Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetFileCompletionNotificationModes, Functions::Overrides::WinBase::SetFileCompletionNotificationModes), "AttachDetours DetourAttach SetFileCompletionNotificationModes Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetFileSecurityA, Functions::Overrides::WinBase::SetFileSecurityA), "AttachDetours DetourAttach SetFileSecurityA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetFileShortNameA, Functions::Overrides::WinBase::SetFileShortNameA), "AttachDetours DetourAttach SetFileShortNameA Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetFileShortNameW, Functions::Overrides::WinBase::SetFileShortNameW), "AttachDetours DetourAttach SetFileShortNameW Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::SetSearchPathMode, Functions::Overrides::WinBase::SetSearchPathMode), "AttachDetours DetourAttach SetSearchPathMode Failed");
		ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WinBase::WriteEncryptedFileRaw, Functions::Overrides::WinBase::WriteEncryptedFileRaw), "AttachDetours DetourAttach WriteEncryptedFileRaw Failed");

		ThrowIfFailed(DetourTransactionCommit(), "AttachDetours DetourTransactionCommit Failed");
	}

	void DetachDetours()
	{
		ThrowIfFailed(DetourTransactionBegin(), "DetachDetours DetourTransactionBegin Failed");
		ThrowIfFailed(DetourUpdateThread(GetCurrentThread()), "DetachDetours DetourUpdateThread Failed");

		// FileApi
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::AreFileApisANSI, Functions::Overrides::FileApi::AreFileApisANSI), "DetachDetours DetourDetach AreFileApisANSI Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::CompareFileTime, Functions::Overrides::FileApi::CompareFileTime), "DetachDetours DetourDetach CompareFileTime Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::CreateDirectoryA, Functions::Overrides::FileApi::CreateDirectoryA), "DetachDetours DetourDetach CreateDirectoryA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::CreateDirectoryW, Functions::Overrides::FileApi::CreateDirectoryW), "DetachDetours DetourDetach CreateDirectoryW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::CreateFile2, Functions::Overrides::FileApi::CreateFile2), "DetachDetours DetourDetach CreateFile2 Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::CreateFileA, Functions::Overrides::FileApi::CreateFileA), "DetachDetours DetourDetach CreateFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::CreateFileW, Functions::Overrides::FileApi::CreateFileW), "DetachDetours DetourDetach CreateFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::DefineDosDeviceW, Functions::Overrides::FileApi::DefineDosDeviceW), "DetachDetours DetourDetach DefineDosDeviceW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::DeleteFileA, Functions::Overrides::FileApi::DeleteFileA), "DetachDetours DetourDetach DeleteFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::DeleteFileW, Functions::Overrides::FileApi::DeleteFileW), "DetachDetours DetourDetach DeleteFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::DeleteVolumeMountPointW, Functions::Overrides::FileApi::DeleteVolumeMountPointW), "DetachDetours DetourDetach DeleteVolumeMountPointW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FileTimeToLocalFileTime, Functions::Overrides::FileApi::FileTimeToLocalFileTime), "DetachDetours DetourDetach FileTimeToLocalFileTime Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindClose, Functions::Overrides::FileApi::FindClose), "DetachDetours DetourDetach FindClose Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindCloseChangeNotification, Functions::Overrides::FileApi::FindCloseChangeNotification), "DetachDetours DetourDetach FindCloseChangeNotification Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstChangeNotificationA, Functions::Overrides::FileApi::FindFirstChangeNotificationA), "DetachDetours DetourDetach FindFirstChangeNotificationA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstChangeNotificationW, Functions::Overrides::FileApi::FindFirstChangeNotificationW), "DetachDetours DetourDetach FindFirstChangeNotificationW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileA, Functions::Overrides::FileApi::FindFirstFileA), "DetachDetours DetourDetach FindFirstFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileW, Functions::Overrides::FileApi::FindFirstFileW), "DetachDetours DetourDetach FindFirstFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileExA, Functions::Overrides::FileApi::FindFirstFileExA), "DetachDetours DetourDetach FindFirstFileExA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileExW, Functions::Overrides::FileApi::FindFirstFileExW), "DetachDetours DetourDetach FindFirstFileExW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstFileNameW, Functions::Overrides::FileApi::FindFirstFileNameW), "DetachDetours DetourDetach FindFirstFileNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstStreamW, Functions::Overrides::FileApi::FindFirstStreamW), "DetachDetours DetourDetach FindFirstStreamW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindFirstVolumeW, Functions::Overrides::FileApi::FindFirstVolumeW), "DetachDetours DetourDetach FindFirstVolumeW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindNextChangeNotification, Functions::Overrides::FileApi::FindNextChangeNotification), "DetachDetours DetourDetach FindNextChangeNotification Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindNextFileA, Functions::Overrides::FileApi::FindNextFileA), "DetachDetours DetourDetach FindNextFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindNextFileW, Functions::Overrides::FileApi::FindNextFileW), "DetachDetours DetourDetach FindNextFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindNextFileNameW, Functions::Overrides::FileApi::FindNextFileNameW), "DetachDetours DetourDetach FindNextFileNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindNextStreamW, Functions::Overrides::FileApi::FindNextStreamW), "DetachDetours DetourDetach FindNextStreamW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindNextVolumeW, Functions::Overrides::FileApi::FindNextVolumeW), "DetachDetours DetourDetach FindNextVolumeW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FindVolumeClose, Functions::Overrides::FileApi::FindVolumeClose), "DetachDetours DetourDetach FindVolumeClose Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::FlushFileBuffers, Functions::Overrides::FileApi::FlushFileBuffers), "DetachDetours DetourDetach FlushFileBuffers Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetCompressedFileSizeA, Functions::Overrides::FileApi::GetCompressedFileSizeA), "DetachDetours DetourDetach GetCompressedFileSizeA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetCompressedFileSizeW, Functions::Overrides::FileApi::GetCompressedFileSizeW), "DetachDetours DetourDetach GetCompressedFileSizeW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetDiskFreeSpaceA, Functions::Overrides::FileApi::GetDiskFreeSpaceA), "DetachDetours DetourDetach GetDiskFreeSpaceA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetDiskFreeSpaceW, Functions::Overrides::FileApi::GetDiskFreeSpaceW), "DetachDetours DetourDetach GetDiskFreeSpaceW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetDiskFreeSpaceExA, Functions::Overrides::FileApi::GetDiskFreeSpaceExA), "DetachDetours DetourDetach GetDiskFreeSpaceExA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetDiskFreeSpaceExW, Functions::Overrides::FileApi::GetDiskFreeSpaceExW), "DetachDetours DetourDetach GetDiskFreeSpaceExW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetDriveTypeA, Functions::Overrides::FileApi::GetDriveTypeA), "DetachDetours DetourDetach GetDriveTypeA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetDriveTypeW, Functions::Overrides::FileApi::GetDriveTypeW), "DetachDetours DetourDetach GetDriveTypeW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileAttributesA, Functions::Overrides::FileApi::GetFileAttributesA), "DetachDetours DetourDetach GetFileAttributesA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileAttributesW, Functions::Overrides::FileApi::GetFileAttributesW), "DetachDetours DetourDetach GetFileAttributesW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileAttributesExA, Functions::Overrides::FileApi::GetFileAttributesExA), "DetachDetours DetourDetach GetFileAttributesExA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileAttributesExW, Functions::Overrides::FileApi::GetFileAttributesExW), "DetachDetours DetourDetach GetFileAttributesExW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileInformationByHandle, Functions::Overrides::FileApi::GetFileInformationByHandle), "DetachDetours DetourDetach GetFileInformationByHandle Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileSize, Functions::Overrides::FileApi::GetFileSize), "DetachDetours DetourDetach GetFileSize Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileSizeEx, Functions::Overrides::FileApi::GetFileSizeEx), "DetachDetours DetourDetach GetFileSizeEx Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileTime, Functions::Overrides::FileApi::GetFileTime), "DetachDetours DetourDetach GetFileTime Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFileType, Functions::Overrides::FileApi::GetFileType), "DetachDetours DetourDetach GetFileType Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFinalPathNameByHandleA, Functions::Overrides::FileApi::GetFinalPathNameByHandleA), "DetachDetours DetourDetach GetFinalPathNameByHandleA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFinalPathNameByHandleW, Functions::Overrides::FileApi::GetFinalPathNameByHandleW), "DetachDetours DetourDetach GetFinalPathNameByHandleW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFullPathNameA, Functions::Overrides::FileApi::GetFullPathNameA), "DetachDetours DetourDetach GetFullPathNameA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetFullPathNameW, Functions::Overrides::FileApi::GetFullPathNameW), "DetachDetours DetourDetach GetFullPathNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetLogicalDrives, Functions::Overrides::FileApi::GetLogicalDrives), "DetachDetours DetourDetach GetLogicalDrives Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetLogicalDriveStringsW, Functions::Overrides::FileApi::GetLogicalDriveStringsW), "DetachDetours DetourDetach GetLogicalDriveStringsW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetLongPathNameA, Functions::Overrides::FileApi::GetLongPathNameA), "DetachDetours DetourDetach GetLongPathNameA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetLongPathNameW, Functions::Overrides::FileApi::GetLongPathNameW), "DetachDetours DetourDetach GetLongPathNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetShortPathNameW, Functions::Overrides::FileApi::GetShortPathNameW), "DetachDetours DetourDetach GetShortPathNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetTempFileNameA, Functions::Overrides::FileApi::GetTempFileNameA), "DetachDetours DetourDetach GetTempFileNameA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetTempFileNameW, Functions::Overrides::FileApi::GetTempFileNameW), "DetachDetours DetourDetach GetTempFileNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetTempPathA, Functions::Overrides::FileApi::GetTempPathA), "DetachDetours DetourDetach GetTempPathA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetTempPathW, Functions::Overrides::FileApi::GetTempPathW), "DetachDetours DetourDetach GetTempPathW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetVolumeInformationA, Functions::Overrides::FileApi::GetVolumeInformationA), "DetachDetours DetourDetach GetVolumeInformationA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetVolumeInformationW, Functions::Overrides::FileApi::GetVolumeInformationW), "DetachDetours DetourDetach GetVolumeInformationW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetVolumeInformationByHandleW, Functions::Overrides::FileApi::GetVolumeInformationByHandleW), "DetachDetours DetourDetach GetVolumeInformationByHandleW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetVolumeNameForVolumeMountPointW, Functions::Overrides::FileApi::GetVolumeNameForVolumeMountPointW), "DetachDetours DetourDetach GetVolumeNameForVolumeMountPointW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetVolumePathNamesForVolumeNameW, Functions::Overrides::FileApi::GetVolumePathNamesForVolumeNameW), "DetachDetours DetourDetach GetVolumePathNamesForVolumeNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::GetVolumePathNameW, Functions::Overrides::FileApi::GetVolumePathNameW), "DetachDetours DetourDetach GetVolumePathNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::LocalFileTimeToFileTime, Functions::Overrides::FileApi::LocalFileTimeToFileTime), "DetachDetours DetourDetach LocalFileTimeToFileTime Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::LockFile, Functions::Overrides::FileApi::LockFile), "DetachDetours DetourDetach LockFile Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::LockFileEx, Functions::Overrides::FileApi::LockFileEx), "DetachDetours DetourDetach LockFileEx Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::QueryDosDeviceW, Functions::Overrides::FileApi::QueryDosDeviceW), "DetachDetours DetourDetach QueryDosDeviceW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::ReadFile, Functions::Overrides::FileApi::ReadFile), "DetachDetours DetourDetach ReadFile Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::ReadFileEx, Functions::Overrides::FileApi::ReadFileEx), "DetachDetours DetourDetach ReadFileEx Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::ReadFileScatter, Functions::Overrides::FileApi::ReadFileScatter), "DetachDetours DetourDetach ReadFileScatter Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::RemoveDirectoryA, Functions::Overrides::FileApi::RemoveDirectoryA), "DetachDetours DetourDetach RemoveDirectoryA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::RemoveDirectoryW, Functions::Overrides::FileApi::RemoveDirectoryW), "DetachDetours DetourDetach RemoveDirectoryW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetEndOfFile, Functions::Overrides::FileApi::SetEndOfFile), "DetachDetours DetourDetach SetEndOfFile Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFileApisToANSI, Functions::Overrides::FileApi::SetFileApisToANSI), "DetachDetours DetourDetach SetFileApisToANSI Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFileApisToOEM, Functions::Overrides::FileApi::SetFileApisToOEM), "DetachDetours DetourDetach SetFileApisToOEM Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFileAttributesA, Functions::Overrides::FileApi::SetFileAttributesA), "DetachDetours DetourDetach SetFileAttributesA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFileAttributesW, Functions::Overrides::FileApi::SetFileAttributesW), "DetachDetours DetourDetach SetFileAttributesW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFileInformationByHandle, Functions::Overrides::FileApi::SetFileInformationByHandle), "DetachDetours DetourDetach SetFileInformationByHandle Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFileIoOverlappedRange, Functions::Overrides::FileApi::SetFileIoOverlappedRange), "DetachDetours DetourDetach SetFileIoOverlappedRange Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFilePointer, Functions::Overrides::FileApi::SetFilePointer), "DetachDetours DetourDetach SetFilePointer Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFilePointerEx, Functions::Overrides::FileApi::SetFilePointerEx), "DetachDetours DetourDetach SetFilePointerEx Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFileTime, Functions::Overrides::FileApi::SetFileTime), "DetachDetours DetourDetach SetFileTime Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::SetFileValidData, Functions::Overrides::FileApi::SetFileValidData), "DetachDetours DetourDetach SetFileValidData Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::UnlockFile, Functions::Overrides::FileApi::UnlockFile), "DetachDetours DetourDetach UnlockFile Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::UnlockFileEx, Functions::Overrides::FileApi::UnlockFileEx), "DetachDetours DetourDetach UnlockFileEx Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::WriteFile, Functions::Overrides::FileApi::WriteFile), "DetachDetours DetourDetach WriteFile Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::WriteFileEx, Functions::Overrides::FileApi::WriteFileEx), "DetachDetours DetourDetach WriteFileEx Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::FileApi::WriteFileGather, Functions::Overrides::FileApi::WriteFileGather), "DetachDetours DetourDetach WriteFileGather Failed");

		// LibLoaderApi
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::LibLoaderApi::LoadLibraryA, Functions::Overrides::LibLoaderApi::LoadLibraryA), "DetachDetours DetourDetach LoadLibraryA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::LibLoaderApi::LoadLibraryW, Functions::Overrides::LibLoaderApi::LoadLibraryW), "DetachDetours DetourDetach LoadLibraryW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::LibLoaderApi::LoadLibraryExA, Functions::Overrides::LibLoaderApi::LoadLibraryExA), "DetachDetours DetourDetach LoadLibraryExA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::LibLoaderApi::LoadLibraryExW, Functions::Overrides::LibLoaderApi::LoadLibraryExW), "DetachDetours DetourDetach LoadLibraryExW Failed");

		// ProcessEnv
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::ProcessEnv::SearchPathA, Functions::Overrides::ProcessEnv::SearchPathA), "DetachDetours DetourDetach SearchPathA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::ProcessEnv::SearchPathW, Functions::Overrides::ProcessEnv::SearchPathW), "DetachDetours DetourDetach SearchPathW Failed");

		// ProcessThreadsApi
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::ProcessThreadsApi::CreateProcessA, Functions::Overrides::ProcessThreadsApi::CreateProcessA), "DetachDetours DetourDetach CreateProcessA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::ProcessThreadsApi::CreateProcessW, Functions::Overrides::ProcessThreadsApi::CreateProcessW), "DetachDetours DetourDetach CreateProcessW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::ProcessThreadsApi::CreateProcessAsUserA, Functions::Overrides::ProcessThreadsApi::CreateProcessAsUserA), "DetachDetours DetourDetach CreateProcessAsUserA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::ProcessThreadsApi::CreateProcessAsUserW, Functions::Overrides::ProcessThreadsApi::CreateProcessAsUserW), "DetachDetours DetourDetach CreateProcessAsUserW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::ProcessThreadsApi::ExitProcess, Functions::Overrides::ProcessThreadsApi::ExitProcess), "DetachDetours DetourDetach ExitProcess Failed");

		// UndocumentedApi
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::UndocumentedApi::EntryPoint, Functions::Overrides::UndocumentedApi::EntryPoint), "DetachDetours DetourDetach EntryPoint Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::UndocumentedApi::PrivCopyFileExA, Functions::Overrides::UndocumentedApi::PrivCopyFileExA), "DetachDetours DetourDetach PrivCopyFileExA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::UndocumentedApi::PrivCopyFileExW, Functions::Overrides::UndocumentedApi::PrivCopyFileExW), "DetachDetours DetourDetach PrivCopyFileExW Failed");

		// WinBase
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CopyFileA, Functions::Overrides::WinBase::CopyFileA), "DetachDetours DetourDetach CopyFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CopyFileW, Functions::Overrides::WinBase::CopyFileW), "DetachDetours DetourDetach CopyFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CopyFile2, Functions::Overrides::WinBase::CopyFile2), "DetachDetours DetourDetach CopyFile2 Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CopyFileExA, Functions::Overrides::WinBase::CopyFileExA), "DetachDetours DetourDetach CopyFileExA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CopyFileExW, Functions::Overrides::WinBase::CopyFileExW), "DetachDetours DetourDetach CopyFileExW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CopyFileTransactedA, Functions::Overrides::WinBase::CopyFileTransactedA), "DetachDetours DetourDetach CopyFileTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CopyFileTransactedW, Functions::Overrides::WinBase::CopyFileTransactedW), "DetachDetours DetourDetach CopyFileTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateDirectoryExA, Functions::Overrides::WinBase::CreateDirectoryExA), "DetachDetours DetourDetach CreateDirectoryExA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateDirectoryExW, Functions::Overrides::WinBase::CreateDirectoryExW), "DetachDetours DetourDetach CreateDirectoryExW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateDirectoryTransactedA, Functions::Overrides::WinBase::CreateDirectoryTransactedA), "DetachDetours DetourDetach CreateDirectoryTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateDirectoryTransactedW, Functions::Overrides::WinBase::CreateDirectoryTransactedW), "DetachDetours DetourDetach CreateDirectoryTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateFileTransactedA, Functions::Overrides::WinBase::CreateFileTransactedA), "DetachDetours DetourDetach CreateFileTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateFileTransactedW, Functions::Overrides::WinBase::CreateFileTransactedW), "DetachDetours DetourDetach CreateFileTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateHardLinkA, Functions::Overrides::WinBase::CreateHardLinkA), "DetachDetours DetourDetach CreateHardLinkA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateHardLinkW, Functions::Overrides::WinBase::CreateHardLinkW), "DetachDetours DetourDetach CreateHardLinkW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateHardLinkTransactedA, Functions::Overrides::WinBase::CreateHardLinkTransactedA), "DetachDetours DetourDetach CreateHardLinkTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateHardLinkTransactedW, Functions::Overrides::WinBase::CreateHardLinkTransactedW), "DetachDetours DetourDetach CreateHardLinkTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateProcessWithLogonW, Functions::Overrides::WinBase::CreateProcessWithLogonW), "DetachDetours DetourDetach CreateProcessWithLogonW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateProcessWithTokenW, Functions::Overrides::WinBase::CreateProcessWithTokenW), "DetachDetours DetourDetach CreateProcessWithTokenW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateSymbolicLinkA, Functions::Overrides::WinBase::CreateSymbolicLinkA), "DetachDetours DetourDetach CreateSymbolicLinkA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateSymbolicLinkW, Functions::Overrides::WinBase::CreateSymbolicLinkW), "DetachDetours DetourDetach CreateSymbolicLinkW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateSymbolicLinkTransactedA, Functions::Overrides::WinBase::CreateSymbolicLinkTransactedA), "DetachDetours DetourDetach CreateSymbolicLinkTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::CreateSymbolicLinkTransactedW, Functions::Overrides::WinBase::CreateSymbolicLinkTransactedW), "DetachDetours DetourDetach CreateSymbolicLinkTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::DecryptFileA, Functions::Overrides::WinBase::DecryptFileA), "DetachDetours DetourDetach DecryptFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::DecryptFileW, Functions::Overrides::WinBase::DecryptFileW), "DetachDetours DetourDetach DecryptFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::DeleteFileTransactedA, Functions::Overrides::WinBase::DeleteFileTransactedA), "DetachDetours DetourDetach DeleteFileTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::DeleteFileTransactedW, Functions::Overrides::WinBase::DeleteFileTransactedW), "DetachDetours DetourDetach DeleteFileTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::EncryptFileA, Functions::Overrides::WinBase::EncryptFileA), "DetachDetours DetourDetach EncryptFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::EncryptFileW, Functions::Overrides::WinBase::EncryptFileW), "DetachDetours DetourDetach EncryptFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::FileEncryptionStatusA, Functions::Overrides::WinBase::FileEncryptionStatusA), "DetachDetours DetourDetach FileEncryptionStatusA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::FileEncryptionStatusW, Functions::Overrides::WinBase::FileEncryptionStatusW), "DetachDetours DetourDetach FileEncryptionStatusW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::FindFirstFileNameTransactedW, Functions::Overrides::WinBase::FindFirstFileNameTransactedW), "DetachDetours DetourDetach FindFirstFileNameTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::FindFirstFileTransactedA, Functions::Overrides::WinBase::FindFirstFileTransactedA), "DetachDetours DetourDetach FindFirstFileTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::FindFirstFileTransactedW, Functions::Overrides::WinBase::FindFirstFileTransactedW), "DetachDetours DetourDetach FindFirstFileTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::FindFirstStreamTransactedW, Functions::Overrides::WinBase::FindFirstStreamTransactedW), "DetachDetours DetourDetach FindFirstStreamTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetBinaryTypeA, Functions::Overrides::WinBase::GetBinaryTypeA), "DetachDetours DetourDetach GetBinaryTypeA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetBinaryTypeW, Functions::Overrides::WinBase::GetBinaryTypeW), "DetachDetours DetourDetach GetBinaryTypeW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetCompressedFileSizeTransactedA, Functions::Overrides::WinBase::GetCompressedFileSizeTransactedA), "DetachDetours DetourDetach GetCompressedFileSizeTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetCompressedFileSizeTransactedW, Functions::Overrides::WinBase::GetCompressedFileSizeTransactedW), "DetachDetours DetourDetach GetCompressedFileSizeTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetDllDirectoryA, Functions::Overrides::WinBase::GetDllDirectoryA), "DetachDetours DetourDetach GetDllDirectoryA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetDllDirectoryW, Functions::Overrides::WinBase::GetDllDirectoryW), "DetachDetours DetourDetach GetDllDirectoryW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetEnvironmentVariableA, Functions::Overrides::WinBase::GetEnvironmentVariableA), "DetachDetours DetourDetach GetEnvironmentVariableA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetEnvironmentVariableW, Functions::Overrides::WinBase::GetEnvironmentVariableW), "DetachDetours DetourDetach GetEnvironmentVariableW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetFileAttributesTransactedA, Functions::Overrides::WinBase::GetFileAttributesTransactedA), "DetachDetours DetourDetach GetFileAttributesTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetFileAttributesTransactedW, Functions::Overrides::WinBase::GetFileAttributesTransactedW), "DetachDetours DetourDetach GetFileAttributesTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetFileBandwidthReservation, Functions::Overrides::WinBase::GetFileBandwidthReservation), "DetachDetours DetourDetach GetFileBandwidthReservation Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetFileInformationByHandleEx, Functions::Overrides::WinBase::GetFileInformationByHandleEx), "DetachDetours DetourDetach GetFileInformationByHandleEx Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetFileSecurityA, Functions::Overrides::WinBase::GetFileSecurityA), "DetachDetours DetourDetach GetFileSecurityA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetFullPathNameTransactedA, Functions::Overrides::WinBase::GetFullPathNameTransactedA), "DetachDetours DetourDetach GetFullPathNameTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetFullPathNameTransactedW, Functions::Overrides::WinBase::GetFullPathNameTransactedW), "DetachDetours DetourDetach GetFullPathNameTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetLongPathNameTransactedA, Functions::Overrides::WinBase::GetLongPathNameTransactedA), "DetachDetours DetourDetach GetLongPathNameTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetLongPathNameTransactedW, Functions::Overrides::WinBase::GetLongPathNameTransactedW), "DetachDetours DetourDetach GetLongPathNameTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetQueuedCompletionStatus, Functions::Overrides::WinBase::GetQueuedCompletionStatus), "DetachDetours DetourDetach GetQueuedCompletionStatus Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetQueuedCompletionStatusEx, Functions::Overrides::WinBase::GetQueuedCompletionStatusEx), "DetachDetours DetourDetach GetQueuedCompletionStatusEx Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::GetShortPathNameA, Functions::Overrides::WinBase::GetShortPathNameA), "DetachDetours DetourDetach GetShortPathNameA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::LoadModule, Functions::Overrides::WinBase::LoadModule), "DetachDetours DetourDetach LoadModule Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::LoadPackagedLibrary, Functions::Overrides::WinBase::LoadPackagedLibrary), "DetachDetours DetourDetach LoadPackagedLibrary Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::MoveFileA, Functions::Overrides::WinBase::MoveFileA), "DetachDetours DetourDetach MoveFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::MoveFileW, Functions::Overrides::WinBase::MoveFileW), "DetachDetours DetourDetach MoveFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::MoveFileExA, Functions::Overrides::WinBase::MoveFileExA), "DetachDetours DetourDetach MoveFileExA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::MoveFileExW, Functions::Overrides::WinBase::MoveFileExW), "DetachDetours DetourDetach MoveFileExW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::MoveFileTransactedA, Functions::Overrides::WinBase::MoveFileTransactedA), "DetachDetours DetourDetach MoveFileTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::MoveFileTransactedW, Functions::Overrides::WinBase::MoveFileTransactedW), "DetachDetours DetourDetach MoveFileTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::MoveFileWithProgressA, Functions::Overrides::WinBase::MoveFileWithProgressA), "DetachDetours DetourDetach MoveFileWithProgressA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::MoveFileWithProgressW, Functions::Overrides::WinBase::MoveFileWithProgressW), "DetachDetours DetourDetach MoveFileWithProgressW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::OpenEncryptedFileRawA, Functions::Overrides::WinBase::OpenEncryptedFileRawA), "DetachDetours DetourDetach OpenEncryptedFileRawA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::OpenEncryptedFileRawW, Functions::Overrides::WinBase::OpenEncryptedFileRawW), "DetachDetours DetourDetach OpenEncryptedFileRawW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::OpenFile, Functions::Overrides::WinBase::OpenFile), "DetachDetours DetourDetach OpenFile Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::OpenFileById, Functions::Overrides::WinBase::OpenFileById), "DetachDetours DetourDetach OpenFileById Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::ReadEncryptedFileRaw, Functions::Overrides::WinBase::ReadEncryptedFileRaw), "DetachDetours DetourDetach ReadEncryptedFileRaw Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::RemoveDirectoryTransactedA, Functions::Overrides::WinBase::RemoveDirectoryTransactedA), "DetachDetours DetourDetach RemoveDirectoryTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::RemoveDirectoryTransactedW, Functions::Overrides::WinBase::RemoveDirectoryTransactedW), "DetachDetours DetourDetach RemoveDirectoryTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::ReOpenFile, Functions::Overrides::WinBase::ReOpenFile), "DetachDetours DetourDetach ReOpenFile Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::ReplaceFileA, Functions::Overrides::WinBase::ReplaceFileA), "DetachDetours DetourDetach ReplaceFileA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::ReplaceFileW, Functions::Overrides::WinBase::ReplaceFileW), "DetachDetours DetourDetach ReplaceFileW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetCurrentDirectoryA, Functions::Overrides::WinBase::SetCurrentDirectoryA), "DetachDetours DetourDetach SetCurrentDirectoryA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetCurrentDirectoryW, Functions::Overrides::WinBase::SetCurrentDirectoryW), "DetachDetours DetourDetach SetCurrentDirectoryW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetDllDirectoryA, Functions::Overrides::WinBase::SetDllDirectoryA), "DetachDetours DetourDetach SetDllDirectoryA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetDllDirectoryW, Functions::Overrides::WinBase::SetDllDirectoryW), "DetachDetours DetourDetach SetDllDirectoryW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetFileAttributesTransactedA, Functions::Overrides::WinBase::SetFileAttributesTransactedA), "DetachDetours DetourDetach SetFileAttributesTransactedA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetFileAttributesTransactedW, Functions::Overrides::WinBase::SetFileAttributesTransactedW), "DetachDetours DetourDetach SetFileAttributesTransactedW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetFileBandwidthReservation, Functions::Overrides::WinBase::SetFileBandwidthReservation), "DetachDetours DetourDetach SetFileBandwidthReservation Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetFileCompletionNotificationModes, Functions::Overrides::WinBase::SetFileCompletionNotificationModes), "DetachDetours DetourDetach SetFileCompletionNotificationModes Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetFileSecurityA, Functions::Overrides::WinBase::SetFileSecurityA), "DetachDetours DetourDetach SetFileSecurityA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetFileShortNameA, Functions::Overrides::WinBase::SetFileShortNameA), "DetachDetours DetourDetach SetFileShortNameA Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetFileShortNameW, Functions::Overrides::WinBase::SetFileShortNameW), "DetachDetours DetourDetach SetFileShortNameW Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::SetSearchPathMode, Functions::Overrides::WinBase::SetSearchPathMode), "DetachDetours DetourDetach SetSearchPathMode Failed");
		ThrowIfFailed(DetourDetach(&(PVOID&)Functions::Cache::WinBase::WriteEncryptedFileRaw, Functions::Overrides::WinBase::WriteEncryptedFileRaw), "DetachDetours DetourDetach WriteEncryptedFileRaw Failed");

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

		PBYTE xCreate = (PBYTE)DetourCodeFromPointer((PVOID)Functions::Cache::ProcessThreadsApi::CreateProcessW, nullptr);
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
			auto workingDirectory = Opal::Path(s_Payload.zWorkingDirectory);
			bool enableAccessChecks = s_Payload.EnableAccessChecks;
			auto allowedReadDirectories = ExtractStringList(s_Payload.zReadAccessDirectories, s_Payload.cReadAccessDirectories);
			auto allowedWriteDirectories = ExtractStringList(s_Payload.zWriteAccessDirectories, s_Payload.cWriteAccessDirectories);

			// Initialize the event pipe
			connectionManager.Initialize(traceProcessId);
			Monitor::FileSystemAccessSandbox::Initialize(
				enableAccessChecks,
				std::move(workingDirectory),
				std::move(allowedReadDirectories),
				std::move(allowedWriteDirectories));

			// Find hidden functions.
			Functions::Cache::UndocumentedApi::PrivCopyFileExA =
				(BOOL (WINAPI *)(LPCSTR, LPCSTR, LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD))
				GetProcAddress(s_hKernel32, "PrivCopyFileExW");
			if (Functions::Cache::UndocumentedApi::PrivCopyFileExA == nullptr)
			{
				throw std::runtime_error("Failed to GetProcAddress PrivCopyFileExA");
			}

			// Find hidden functions.
			Functions::Cache::UndocumentedApi::PrivCopyFileExW =
				(BOOL (WINAPI *)(LPCWSTR, LPCWSTR, LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD))
				GetProcAddress(s_hKernel32, "PrivCopyFileExW");
			if (Functions::Cache::UndocumentedApi::PrivCopyFileExW == nullptr)
			{
				throw std::runtime_error("Failed to GetProcAddress PrivCopyFileExW");
			}

			AttachDetours();
		}
		catch (const std::exception& ex)
		{
			connectionManager.WriteError(ex.what());
			exit(-1234);
		}
		catch (...)
		{
			connectionManager.WriteError("Unknown error attaching detours");
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
			connectionManager.WriteError(ex.what());
			exit(-1234);
		}
		catch (...)
		{
			connectionManager.WriteError("Unknown error detaching detours");
			exit(-1234);
		}

		connectionManager.Shutdown();

		return true;
	}
}