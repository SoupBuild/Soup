#pragma once 

namespace Monitor
{
	class DetourCallbackLogger : public IDetourCallback
	{
	public:
		DetourCallbackLogger(std::ostream& stream) :
			m_stream(stream)
		{
		}

		void OnShutdown() override final
		{
			m_stream << "Shutdown: " << std::endl;
		}

		void OnError(std::string_view message) override final
		{
			m_stream << "Error: " << message << std::endl;
		}

		// FileApi
		void OnAreFileApisANSI() override final
		{
			m_stream << "AreFileApisANSI: " << std::endl;
		}

		void OnCompareFileTime() override final
		{
			m_stream << "CompareFileTime: " << std::endl;
		}

		void OnCreateDirectoryA() override final
		{
			m_stream << "CreateDirectoryA: " << std::endl;
		}

		void OnCreateDirectoryW() override final
		{
			m_stream << "CreateDirectoryW: " << std::endl;
		}

		void OnCreateFile2() override final
		{
			m_stream << "CreateFile2: " << std::endl;
		}

		void OnCreateFileA() override final
		{
			m_stream << "CreateFileA: " << std::endl;
		}

		void OnCreateFileW() override final
		{
			m_stream << "CreateFileW: " << std::endl;
		}

		void OnDefineDosDeviceW() override final
		{
			m_stream << "DefineDosDeviceW: " << std::endl;
		}

		void OnDeleteFileA() override final
		{
			m_stream << "DeleteFileA: " << std::endl;
		}

		void OnDeleteFileW() override final
		{
			m_stream << "DeleteFileW: " << std::endl;
		}

		void OnDeleteVolumeMountPointW() override final
		{
			m_stream << "DeleteVolumeMountPointW: " << std::endl;
		}

		void OnFileTimeToLocalFileTime() override final
		{
			m_stream << "FileTimeToLocalFileTime: " << std::endl;
		}

		void OnFindClose() override final
		{
			m_stream << "FindClose: " << std::endl;
		}

		void OnFindCloseChangeNotification() override final
		{
			m_stream << "FindCloseChangeNotification: " << std::endl;
		}

		void OnFindFirstChangeNotificationA() override final
		{
			m_stream << "FindFirstChangeNotificationA: " << std::endl;
		}

		void OnFindFirstChangeNotificationW() override final
		{
			m_stream << "FindFirstChangeNotificationW: " << std::endl;
		}

		void OnFindFirstFileA() override final
		{
			m_stream << "FindFirstFileA: " << std::endl;
		}

		void OnFindFirstFileW() override final
		{
			m_stream << "FindFirstFileW: " << std::endl;
		}

		void OnFindFirstFileExA() override final
		{
			m_stream << "FindFirstFileExA: " << std::endl;
		}

		void OnFindFirstFileExW() override final
		{
			m_stream << "FindFirstFileExW: " << std::endl;
		}

		void OnFindFirstFileNameW() override final
		{
			m_stream << "FindFirstFileNameW: " << std::endl;
		}

		void OnFindFirstStreamW() override final
		{
			m_stream << "FindFirstStreamW: " << std::endl;
		}

		void OnFindFirstVolumeW() override final
		{
			m_stream << "FindFirstVolumeW: " << std::endl;
		}

		void OnFindNextChangeNotification() override final
		{
			m_stream << "FindNextChangeNotification: " << std::endl;
		}

		void OnFindNextFileA() override final
		{
			m_stream << "FindNextFileA: " << std::endl;
		}

		void OnFindNextFileW() override final
		{
			m_stream << "FindNextFileW: " << std::endl;
		}

		void OnFindNextFileNameW() override final
		{
			m_stream << "FindNextFileNameW: " << std::endl;
		}

		void OnFindNextStreamW() override final
		{
			m_stream << "FindNextStreamW: " << std::endl;
		}

		void OnFindNextVolumeW() override final
		{
			m_stream << "FindNextVolumeW: " << std::endl;
		}

		void OnFindVolumeClose() override final
		{
			m_stream << "FindVolumeClose: " << std::endl;
		}

		void OnFlushFileBuffers() override final
		{
			m_stream << "FlushFileBuffers: " << std::endl;
		}

		void OnGetCompressedFileSizeA() override final
		{
			m_stream << "GetCompressedFileSizeA: " << std::endl;
		}

		void OnGetCompressedFileSizeW() override final
		{
			m_stream << "GetCompressedFileSizeW: " << std::endl;
		}

		void OnGetDiskFreeSpaceA() override final
		{
			m_stream << "GetDiskFreeSpaceA: " << std::endl;
		}

		void OnGetDiskFreeSpaceW() override final
		{
			m_stream << "GetDiskFreeSpaceW: " << std::endl;
		}

		void OnGetDiskFreeSpaceExA() override final
		{
			m_stream << "GetDiskFreeSpaceExA: " << std::endl;
		}

		void OnGetDiskFreeSpaceExW() override final
		{
			m_stream << "GetDiskFreeSpaceExW: " << std::endl;
		}

		void OnGetDriveTypeA() override final
		{
			m_stream << "GetDriveTypeA: " << std::endl;
		}

		void OnGetDriveTypeW() override final
		{
			m_stream << "GetDriveTypeW: " << std::endl;
		}

		void OnGetFileAttributesA() override final
		{
			m_stream << "GetFileAttributesA: " << std::endl;
		}

		void OnGetFileAttributesW() override final
		{
			m_stream << "GetFileAttributesW: " << std::endl;
		}

		void OnGetFileAttributesExA() override final
		{
			m_stream << "GetFileAttributesExA: " << std::endl;
		}

		void OnGetFileAttributesExW() override final
		{
			m_stream << "GetFileAttributesExW: " << std::endl;
		}

		void OnGetFileInformationByHandle() override final
		{
			m_stream << "GetFileInformationByHandle: " << std::endl;
		}

		void OnGetFileSize() override final
		{
			m_stream << "GetFileSize: " << std::endl;
		}

		void OnGetFileSizeEx() override final
		{
			m_stream << "GetFileSizeEx: " << std::endl;
		}

		void OnGetFileTime() override final
		{
			m_stream << "GetFileTime: " << std::endl;
		}

		void OnGetFileType() override final
		{
			m_stream << "GetFileType: " << std::endl;
		}

		void OnGetFinalPathNameByHandleA() override final
		{
			m_stream << "GetFinalPathNameByHandleA: " << std::endl;
		}

		void OnGetFinalPathNameByHandleW() override final
		{
			m_stream << "GetFinalPathNameByHandleW: " << std::endl;
		}

		void OnGetFullPathNameA() override final
		{
			m_stream << "GetFullPathNameA: " << std::endl;
		}

		void OnGetFullPathNameW() override final
		{
			m_stream << "GetFullPathNameW: " << std::endl;
		}

		void OnGetLogicalDrives() override final
		{
			m_stream << "GetLogicalDrives: " << std::endl;
		}

		void OnGetLogicalDriveStringsW() override final
		{
			m_stream << "GetLogicalDriveStringsW: " << std::endl;
		}

		void OnGetLongPathNameA() override final
		{
			m_stream << "GetLongPathNameA: " << std::endl;
		}

		void OnGetLongPathNameW() override final
		{
			m_stream << "GetLongPathNameW: " << std::endl;
		}

		void OnGetShortPathNameW() override final
		{
			m_stream << "GetShortPathNameW: " << std::endl;
		}

		void OnGetTempFileNameA() override final
		{
			m_stream << "GetTempFileNameA: " << std::endl;
		}

		void OnGetTempFileNameW() override final
		{
			m_stream << "GetTempFileNameW: " << std::endl;
		}

		void OnGetTempPathA() override final
		{
			m_stream << "GetTempPathA: " << std::endl;
		}

		void OnGetTempPathW() override final
		{
			m_stream << "GetTempPathW: " << std::endl;
		}

		void OnGetVolumeInformationA() override final
		{
			m_stream << "GetVolumeInformationA: " << std::endl;
		}

		void OnGetVolumeInformationW() override final
		{
			m_stream << "GetVolumeInformationW: " << std::endl;
		}

		void OnGetVolumeInformationByHandleW() override final
		{
			m_stream << "GetVolumeInformationByHandleW: " << std::endl;
		}

		void OnGetVolumeNameForVolumeMountPointW() override final
		{
			m_stream << "GetVolumeNameForVolumeMountPointW: " << std::endl;
		}

		void OnGetVolumePathNamesForVolumeNameW() override final
		{
			m_stream << "GetVolumePathNamesForVolumeNameW: " << std::endl;
		}

		void OnGetVolumePathNameW() override final
		{
			m_stream << "GetVolumePathNameW: " << std::endl;
		}

		void OnLocalFileTimeToFileTime() override final
		{
			m_stream << "LocalFileTimeToFileTime: " << std::endl;
		}

		void OnLockFile() override final
		{
			m_stream << "LockFile: " << std::endl;
		}

		void OnLockFileEx() override final
		{
			m_stream << "LockFileEx: " << std::endl;
		}

		void OnQueryDosDeviceW() override final
		{
			m_stream << "QueryDosDeviceW: " << std::endl;
		}

		void OnReadFile() override final
		{
			m_stream << "ReadFile: " << std::endl;
		}

		void OnReadFileEx() override final
		{
			m_stream << "ReadFileEx: " << std::endl;
		}

		void OnReadFileScatter() override final
		{
			m_stream << "ReadFileScatter: " << std::endl;
		}

		void OnRemoveDirectoryA() override final
		{
			m_stream << "RemoveDirectoryA: " << std::endl;
		}

		void OnRemoveDirectoryW() override final
		{
			m_stream << "RemoveDirectoryW: " << std::endl;
		}

		void OnSetEndOfFile() override final
		{
			m_stream << "SetEndOfFile: " << std::endl;
		}
		
		void OnSetFileApisToANSI() override final
		{
			m_stream << "SetFileApisToANSI: " << std::endl;
		}

		void OnSetFileApisToOEM() override final
		{
			m_stream << "SetFileApisToOEM: " << std::endl;
		}

		void OnSetFileAttributesA() override final
		{
			m_stream << "SetFileAttributesA: " << std::endl;
		}
		
		void OnSetFileAttributesW() override final
		{
			m_stream << "SetFileAttributesW: " << std::endl;
		}

		void OnSetFileInformationByHandle() override final
		{
			m_stream << "SetFileInformationByHandle: " << std::endl;
		}

		void OnSetFileIoOverlappedRange() override final
		{
			m_stream << "SetFileIoOverlappedRange: " << std::endl;
		}

		void OnSetFilePointer() override final
		{
			m_stream << "SetFilePointer: " << std::endl;
		}
		
		void OnSetFilePointerEx() override final
		{
			m_stream << "SetFilePointerEx: " << std::endl;
		}

		void OnSetFileTime() override final
		{
			m_stream << "SetFileTime: " << std::endl;
		}

		void OnSetFileValidData() override final
		{
			m_stream << "SetFileValidData: " << std::endl;
		}

		void OnUnlockFile() override final
		{
			m_stream << "UnlockFile: " << std::endl;
		}

		void OnUnlockFileEx() override final
		{
			m_stream << "UnlockFileEx: " << std::endl;
		}

		void OnWriteFile() override final
		{
			m_stream << "WriteFile: " << std::endl;
		}

		void OnWriteFileEx() override final
		{
			m_stream << "WriteFileEx: " << std::endl;
		}

		void OnWriteFileGather() override final
		{
			m_stream << "WriteFileGather: " << std::endl;
		}

		// LibLoaderApi
		void OnLoadLibraryA() override final
		{
			m_stream << "LoadLibraryA: " << std::endl;
		}

		void OnLoadLibraryW() override final
		{
			m_stream << "LoadLibraryW: " << std::endl;
		}

		void OnLoadLibraryExA() override final
		{
			m_stream << "LoadLibraryExA: " << std::endl;
		}

		void OnLoadLibraryExW() override final
		{
			m_stream << "LoadLibraryExW: " << std::endl;
		}

		// ProcessEnv
		void OnSearchPathA() override final
		{
			m_stream << "SearchPathA: " << std::endl;
		}

		void OnSearchPathW() override final
		{
			m_stream << "SearchPathW: " << std::endl;
		}

		// ProcessThreadsApi
		void OnCreateProcessA() override final
		{
			m_stream << "CreateProcessA: " << std::endl;
		}

		void OnCreateProcessW() override final
		{
			m_stream << "CreateProcessW: " << std::endl;
		}

		void OnCreateProcessAsUserA() override final
		{
			m_stream << "CreateProcessAsUserA: " << std::endl;
		}

		void OnCreateProcessAsUserW() override final
		{
			m_stream << "CreateProcessAsUserW: " << std::endl;
		}

		void OnExitProcess() override final
		{
			m_stream << "ExitProcess: " << std::endl;
		}

		// UndocumentedApi
		void OnPrivCopyFileExA() override final
		{
			m_stream << "PrivCopyFileExA: " << std::endl;
		}

		void OnPrivCopyFileExW() override final
		{
			m_stream << "PrivCopyFileExW: " << std::endl;
		}

		// WinBase
		void OnCopyFileA() override final
		{
			m_stream << "CopyFileA: " << std::endl;
		}

		void OnCopyFileW() override final
		{
			m_stream << "CopyFileW: " << std::endl;
		}

		void OnCopyFile2() override final
		{
			m_stream << "CopyFile2: " << std::endl;
		}

		void OnCopyFileExA() override final
		{
			m_stream << "CopyFileExA: " << std::endl;
		}

		void OnCopyFileExW() override final
		{
			m_stream << "CopyFileExW: " << std::endl;
		}

		void OnCopyFileTransactedA() override final
		{
			m_stream << "CopyFileTransactedA: " << std::endl;
		}

		void OnCopyFileTransactedW() override final
		{
			m_stream << "CopyFileTransactedW: " << std::endl;
		}

		void OnCreateDirectoryExA() override final
		{
			m_stream << "CreateDirectoryExA: " << std::endl;
		}

		void OnCreateDirectoryExW() override final
		{
			m_stream << "CreateDirectoryExW: " << std::endl;
		}

		void OnCreateDirectoryTransactedA() override final
		{
			m_stream << "CreateDirectoryTransactedA: " << std::endl;
		}

		void OnCreateDirectoryTransactedW() override final
		{
			m_stream << "CreateDirectoryTransactedW: " << std::endl;
		}

		void OnCreateFileTransactedA() override final
		{
			m_stream << "CreateFileTransactedA: " << std::endl;
		}

		void OnCreateFileTransactedW() override final
		{
			m_stream << "CreateFileTransactedW: " << std::endl;
		}

		void OnCreateHardLinkA() override final
		{
			m_stream << "CreateHardLinkA: " << std::endl;
		}

		void OnCreateHardLinkW() override final
		{
			m_stream << "CreateHardLinkW: " << std::endl;
		}

		void OnCreateHardLinkTransactedA() override final
		{
			m_stream << "CreateHardLinkTransactedA: " << std::endl;
		}

		void OnCreateHardLinkTransactedW() override final
		{
			m_stream << "CreateHardLinkTransactedW: " << std::endl;
		}

		void OnCreateProcessWithLogonW() override final
		{
			m_stream << "CreateProcessWithLogonW: " << std::endl;
		}

		void OnCreateProcessWithTokenW() override final
		{
			m_stream << "CreateProcessWithTokenW: " << std::endl;
		}

		void OnCreateSymbolicLinkA() override final
		{
			m_stream << "CreateSymbolicLinkA: " << std::endl;
		}

		void OnCreateSymbolicLinkW() override final
		{
			m_stream << "CreateSymbolicLinkW: " << std::endl;
		}

		void OnCreateSymbolicLinkTransactedA() override final
		{
			m_stream << "CreateSymbolicLinkTransactedA: " << std::endl;
		}

		void OnCreateSymbolicLinkTransactedW() override final
		{
			m_stream << "CreateSymbolicLinkTransactedW: " << std::endl;
		}

		void OnDecryptFileA() override final
		{
			m_stream << "DecryptFileA: " << std::endl;
		}
		
		void OnDecryptFileW() override final
		{
			m_stream << "DecryptFileW: " << std::endl;
		}

		void OnDeleteFileTransactedA() override final
		{
			m_stream << "DeleteFileTransactedA: " << std::endl;
		}

		void OnDeleteFileTransactedW() override final
		{
			m_stream << "DeleteFileTransactedW: " << std::endl;
		}

		void OnEncryptFileA() override final
		{
			m_stream << "EncryptFileA: " << std::endl;
		}

		void OnEncryptFileW() override final
		{
			m_stream << "EncryptFileW: " << std::endl;
		}

		void OnFileEncryptionStatusA() override final
		{
			m_stream << "FileEncryptionStatusA: " << std::endl;
		}

		void OnFileEncryptionStatusW() override final
		{
			m_stream << "FileEncryptionStatusW: " << std::endl;
		}

		void OnFindFirstFileNameTransactedW() override final
		{
			m_stream << "FindFirstFileNameTransactedW: " << std::endl;
		}

		void OnFindFirstFileTransactedA() override final
		{
			m_stream << "FindFirstFileTransactedA: " << std::endl;
		}

		void OnFindFirstFileTransactedW() override final
		{
			m_stream << "FindFirstFileTransactedW: " << std::endl;
		}

		void OnFindFirstStreamTransactedW() override final
		{
			m_stream << "FindFirstStreamTransactedW: " << std::endl;
		}

		void OnGetBinaryTypeA() override final
		{
			m_stream << "GetBinaryTypeA: " << std::endl;
		}

		void OnGetBinaryTypeW() override final
		{
			m_stream << "GetBinaryTypeW: " << std::endl;
		}

		void OnGetCompressedFileSizeTransactedA() override final
		{
			m_stream << "GetCompressedFileSizeTransactedA: " << std::endl;
		}

		void OnGetCompressedFileSizeTransactedW() override final
		{
			m_stream << "GetCompressedFileSizeTransactedW: " << std::endl;
		}

		void OnGetDllDirectoryA() override final
		{
			m_stream << "GetDllDirectoryA: " << std::endl;
		}

		void OnGetDllDirectoryW() override final
		{
			m_stream << "GetDllDirectoryW: " << std::endl;
		}

		void OnGetFileAttributesTransactedA() override final
		{
			m_stream << "GetFileAttributesTransactedA: " << std::endl;
		}

		void OnGetFileAttributesTransactedW() override final
		{
			m_stream << "GetFileAttributesTransactedW: " << std::endl;
		}

		void OnGetFileBandwidthReservation() override final
		{
			m_stream << "GetFileBandwidthReservation: " << std::endl;
		}
		
		void OnGetFileInformationByHandleEx() override final
		{
			m_stream << "GetFileInformationByHandleEx: " << std::endl;
		}

		void OnGetFileSecurityA() override final
		{
			m_stream << "GetFileSecurityA: " << std::endl;
		}

		void OnGetFullPathNameTransactedA() override final
		{
			m_stream << "GetFullPathNameTransactedA: " << std::endl;
		}

		void OnGetFullPathNameTransactedW() override final
		{
			m_stream << "GetFullPathNameTransactedW: " << std::endl;
		}

		void OnGetLongPathNameTransactedA() override final
		{
			m_stream << "GetLongPathNameTransactedA: " << std::endl;
		}

		void OnGetLongPathNameTransactedW() override final
		{
			m_stream << "GetLongPathNameTransactedW: " << std::endl;
		}

		void OnGetQueuedCompletionStatus() override final
		{
			m_stream << "GetQueuedCompletionStatus: " << std::endl;
		}

		void OnGetQueuedCompletionStatusEx() override final
		{
			m_stream << "GetQueuedCompletionStatusEx: " << std::endl;
		}

		void OnGetShortPathNameA() override final
		{
			m_stream << "GetShortPathNameA: " << std::endl;
		}

		void OnLoadModule() override final
		{
			m_stream << "LoadModule: " << std::endl;
		}

		void OnLoadPackagedLibrary() override final
		{
			m_stream << "LoadPackagedLibrary: " << std::endl;
		}

		void OnMoveFileA() override final
		{
			m_stream << "MoveFileA: " << std::endl;
		}

		void OnMoveFileW() override final
		{
			m_stream << "MoveFileW: " << std::endl;
		}

		void OnMoveFileExA() override final
		{
			m_stream << "MoveFileExA: " << std::endl;
		}

		void OnMoveFileExW() override final
		{
			m_stream << "MoveFileExW: " << std::endl;
		}

		void OnMoveFileTransactedA() override final
		{
			m_stream << "MoveFileTransactedA: " << std::endl;
		}

		void OnMoveFileTransactedW() override final
		{
			m_stream << "MoveFileTransactedW: " << std::endl;
		}

		void OnMoveFileWithProgressA() override final
		{
			m_stream << "MoveFileWithProgressA: " << std::endl;
		}

		void OnMoveFileWithProgressW() override final
		{
			m_stream << "MoveFileWithProgressW: " << std::endl;
		}

		void OnOpenEncryptedFileRawA() override final
		{
			m_stream << "OpenEncryptedFileRawA: " << std::endl;
		}

		void OnOpenEncryptedFileRawW() override final
		{
			m_stream << "OpenEncryptedFileRawW: " << std::endl;
		}

		void OnOpenFile() override final
		{
			m_stream << "OpenFile: " << std::endl;
		}

		void OnOpenFileById() override final
		{
			m_stream << "OpenFileById: " << std::endl;
		}

		void OnReadEncryptedFileRaw() override final
		{
			m_stream << "ReadEncryptedFileRaw: " << std::endl;
		}

		void OnRemoveDirectoryTransactedA() override final
		{
			m_stream << "RemoveDirectoryTransactedA: " << std::endl;
		}

		void OnRemoveDirectoryTransactedW() override final
		{
			m_stream << "RemoveDirectoryTransactedW: " << std::endl;
		}

		void OnReOpenFile() override final
		{
			m_stream << "ReOpenFile: " << std::endl;
		}

		void OnReplaceFileA() override final
		{
			m_stream << "ReplaceFileA: " << std::endl;
		}

		void OnReplaceFileW() override final
		{
			m_stream << "ReplaceFileW: " << std::endl;
		}

		void OnSetCurrentDirectoryA() override final
		{
			m_stream << "SetCurrentDirectoryA: " << std::endl;
		}

		void OnSetCurrentDirectoryW() override final
		{
			m_stream << "SetCurrentDirectoryW: " << std::endl;
		}

		void OnSetDllDirectoryA() override final
		{
			m_stream << "SetDllDirectoryA: " << std::endl;
		}

		void OnSetDllDirectoryW() override final
		{
			m_stream << "SetDllDirectoryW: " << std::endl;
		}

		void OnSetFileAttributesTransactedA() override final
		{
			m_stream << "SetFileAttributesTransactedA: " << std::endl;
		}

		void OnSetFileAttributesTransactedW() override final
		{
			m_stream << "SetFileAttributesTransactedW: " << std::endl;
		}

		void OnSetFileBandwidthReservation() override final
		{
			m_stream << "SetFileBandwidthReservation: " << std::endl;
		}

		void OnSetFileCompletionNotificationModes() override final
		{
			m_stream << "SetFileCompletionNotificationModes: " << std::endl;
		}

		void OnSetFileSecurityA() override final
		{
			m_stream << "SetFileSecurityA: " << std::endl;
		}

		void OnSetFileShortNameA() override final
		{
			m_stream << "SetFileShortNameA: " << std::endl;
		}

		void OnSetFileShortNameW() override final
		{
			m_stream << "SetFileShortNameW: " << std::endl;
		}

		void OnSetSearchPathMode() override final
		{
			m_stream << "SetSearchPathMode: " << std::endl;
		}

		void OnWriteEncryptedFileRaw() override final
		{
			m_stream << "WriteEncryptedFileRaw: " << std::endl;
		}

	private:
		std::ostream& m_stream;
	};
}
