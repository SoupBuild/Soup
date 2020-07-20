#pragma once 

namespace Soup::Build
{
	class SystemAccessTracker : public Monitor::IDetourCallback
	{
	public:
		SystemAccessTracker() :
			m_input(),
			m_output()
		{
		}

		void OnShutdown() override final
		{

		}

		void OnError(std::string_view message) override final
		{
			Log::Error("SystemAccessTracker Error");
		}

		// FileApi
		void OnAreFileApisANSI() override final
		{

		}

		void OnCompareFileTime() override final
		{

		}

		void OnCreateDirectoryA() override final
		{

		}

		void OnCreateDirectoryW() override final
		{

		}

		void OnCreateFile2() override final
		{

		}

		void OnCreateFileA() override final
		{

		}

		void OnCreateFileW() override final
		{

		}

		void OnDefineDosDeviceW() override final
		{

		}

		void OnDeleteFileA() override final
		{

		}

		void OnDeleteFileW() override final
		{

		}

		void OnDeleteVolumeMountPointW() override final
		{

		}

		void OnFileTimeToLocalFileTime() override final
		{

		}

		void OnFindClose() override final
		{

		}

		void OnFindCloseChangeNotification() override final
		{

		}

		void OnFindFirstChangeNotificationA() override final
		{

		}

		void OnFindFirstChangeNotificationW() override final
		{

		}

		void OnFindFirstFileA() override final
		{

		}

		void OnFindFirstFileW() override final
		{

		}

		void OnFindFirstFileExA() override final
		{

		}

		void OnFindFirstFileExW() override final
		{

		}

		void OnFindFirstFileNameW() override final
		{

		}

		void OnFindFirstStreamW() override final
		{

		}

		void OnFindFirstVolumeW() override final
		{

		}

		void OnFindNextChangeNotification() override final
		{

		}

		void OnFindNextFileA() override final
		{

		}

		void OnFindNextFileW() override final
		{

		}

		void OnFindNextFileNameW() override final
		{

		}

		void OnFindNextStreamW() override final
		{

		}

		void OnFindNextVolumeW() override final
		{

		}

		void OnFindVolumeClose() override final
		{

		}

		void OnFlushFileBuffers() override final
		{

		}

		void OnGetCompressedFileSizeA() override final
		{

		}

		void OnGetCompressedFileSizeW() override final
		{

		}

		void OnGetDiskFreeSpaceA() override final
		{

		}

		void OnGetDiskFreeSpaceW() override final
		{

		}

		void OnGetDiskFreeSpaceExA() override final
		{

		}

		void OnGetDiskFreeSpaceExW() override final
		{

		}

		void OnGetDriveTypeA() override final
		{

		}

		void OnGetDriveTypeW() override final
		{

		}

		void OnGetFileAttributesA() override final
		{

		}

		void OnGetFileAttributesW() override final
		{

		}

		void OnGetFileAttributesExA() override final
		{

		}

		void OnGetFileAttributesExW() override final
		{

		}

		void OnGetFileInformationByHandle() override final
		{

		}

		void OnGetFileSize() override final
		{

		}

		void OnGetFileSizeEx() override final
		{

		}

		void OnGetFileTime() override final
		{

		}

		void OnGetFileType() override final
		{

		}

		void OnGetFinalPathNameByHandleA() override final
		{

		}

		void OnGetFinalPathNameByHandleW() override final
		{

		}

		void OnGetFullPathNameA() override final
		{

		}

		void OnGetFullPathNameW() override final
		{

		}

		void OnGetLogicalDrives() override final
		{

		}

		void OnGetLogicalDriveStringsW() override final
		{

		}

		void OnGetLongPathNameA() override final
		{

		}

		void OnGetLongPathNameW() override final
		{

		}

		void OnGetShortPathNameW() override final
		{

		}

		void OnGetTempFileNameA() override final
		{

		}

		void OnGetTempFileNameW() override final
		{

		}

		void OnGetTempPathA() override final
		{

		}

		void OnGetTempPathW() override final
		{

		}

		void OnGetVolumeInformationA() override final
		{

		}

		void OnGetVolumeInformationW() override final
		{

		}

		void OnGetVolumeInformationByHandleW() override final
		{

		}

		void OnGetVolumeNameForVolumeMountPointW() override final
		{

		}

		void OnGetVolumePathNamesForVolumeNameW() override final
		{

		}

		void OnGetVolumePathNameW() override final
		{

		}

		void OnLocalFileTimeToFileTime() override final
		{

		}

		void OnLockFile() override final
		{

		}

		void OnLockFileEx() override final
		{

		}

		void OnQueryDosDeviceW() override final
		{

		}

		void OnReadFile() override final
		{

		}

		void OnReadFileEx() override final
		{

		}

		void OnReadFileScatter() override final
		{

		}

		void OnRemoveDirectoryA() override final
		{

		}

		void OnRemoveDirectoryW() override final
		{

		}

		void OnSetEndOfFile() override final
		{

		}
		
		void OnSetFileApisToANSI() override final
		{

		}

		void OnSetFileApisToOEM() override final
		{

		}

		void OnSetFileAttributesA() override final
		{

		}
		
		void OnSetFileAttributesW() override final
		{

		}

		void OnSetFileInformationByHandle() override final
		{

		}

		void OnSetFileIoOverlappedRange() override final
		{

		}

		void OnSetFilePointer() override final
		{

		}
		
		void OnSetFilePointerEx() override final
		{

		}

		void OnSetFileTime() override final
		{

		}

		void OnSetFileValidData() override final
		{

		}

		void OnUnlockFile() override final
		{

		}

		void OnUnlockFileEx() override final
		{

		}

		void OnWriteFile() override final
		{

		}

		void OnWriteFileEx() override final
		{

		}

		void OnWriteFileGather() override final
		{

		}

		// LibLoaderApi
		void OnLoadLibraryA() override final
		{

		}

		void OnLoadLibraryW() override final
		{

		}

		void OnLoadLibraryExA() override final
		{

		}

		void OnLoadLibraryExW() override final
		{

		}

		// ProcessEnv
		void OnSearchPathA() override final
		{

		}

		void OnSearchPathW() override final
		{

		}

		// ProcessThreadsApi
		void OnCreateProcessA() override final
		{

		}

		void OnCreateProcessW() override final
		{

		}

		void OnCreateProcessAsUserA() override final
		{

		}

		void OnCreateProcessAsUserW() override final
		{

		}

		void OnExitProcess() override final
		{

		}

		// UndocumentedApi
		void OnPrivCopyFileExA() override final
		{

		}

		void OnPrivCopyFileExW() override final
		{

		}

		// WinBase
		void OnCopyFileA() override final
		{

		}

		void OnCopyFileW() override final
		{

		}

		void OnCopyFile2() override final
		{

		}

		void OnCopyFileExA() override final
		{

		}

		void OnCopyFileExW() override final
		{

		}

		void OnCopyFileTransactedA() override final
		{

		}

		void OnCopyFileTransactedW() override final
		{

		}

		void OnCreateDirectoryExA() override final
		{

		}

		void OnCreateDirectoryExW() override final
		{

		}

		void OnCreateDirectoryTransactedA() override final
		{

		}

		void OnCreateDirectoryTransactedW() override final
		{

		}

		void OnCreateFileTransactedA() override final
		{

		}

		void OnCreateFileTransactedW() override final
		{

		}

		void OnCreateHardLinkA() override final
		{

		}

		void OnCreateHardLinkW() override final
		{

		}

		void OnCreateHardLinkTransactedA() override final
		{

		}

		void OnCreateHardLinkTransactedW() override final
		{

		}

		void OnCreateProcessWithLogonW() override final
		{

		}

		void OnCreateProcessWithTokenW() override final
		{

		}

		void OnCreateSymbolicLinkA() override final
		{

		}

		void OnCreateSymbolicLinkW() override final
		{

		}

		void OnCreateSymbolicLinkTransactedA() override final
		{

		}

		void OnCreateSymbolicLinkTransactedW() override final
		{

		}

		void OnDecryptFileA() override final
		{

		}
		
		void OnDecryptFileW() override final
		{

		}

		void OnDeleteFileTransactedA() override final
		{

		}

		void OnDeleteFileTransactedW() override final
		{

		}

		void OnEncryptFileA() override final
		{

		}

		void OnEncryptFileW() override final
		{

		}

		void OnFileEncryptionStatusA() override final
		{

		}

		void OnFileEncryptionStatusW() override final
		{

		}

		void OnFindFirstFileNameTransactedW() override final
		{

		}

		void OnFindFirstFileTransactedA() override final
		{

		}

		void OnFindFirstFileTransactedW() override final
		{

		}

		void OnFindFirstStreamTransactedW() override final
		{

		}

		void OnGetBinaryTypeA() override final
		{

		}

		void OnGetBinaryTypeW() override final
		{

		}

		void OnGetCompressedFileSizeTransactedA() override final
		{

		}

		void OnGetCompressedFileSizeTransactedW() override final
		{

		}

		void OnGetDllDirectoryA() override final
		{

		}

		void OnGetDllDirectoryW() override final
		{

		}

		void OnGetFileAttributesTransactedA() override final
		{

		}

		void OnGetFileAttributesTransactedW() override final
		{

		}

		void OnGetFileBandwidthReservation() override final
		{

		}
		
		void OnGetFileInformationByHandleEx() override final
		{

		}

		void OnGetFileSecurityA() override final
		{

		}

		void OnGetFullPathNameTransactedA() override final
		{

		}

		void OnGetFullPathNameTransactedW() override final
		{

		}

		void OnGetLongPathNameTransactedA() override final
		{

		}

		void OnGetLongPathNameTransactedW() override final
		{

		}

		void OnGetQueuedCompletionStatus() override final
		{

		}

		void OnGetQueuedCompletionStatusEx() override final
		{

		}

		void OnGetShortPathNameA() override final
		{

		}

		void OnLoadModule() override final
		{

		}

		void OnLoadPackagedLibrary() override final
		{

		}

		void OnMoveFileA() override final
		{

		}

		void OnMoveFileW() override final
		{

		}

		void OnMoveFileExA() override final
		{

		}

		void OnMoveFileExW() override final
		{

		}

		void OnMoveFileTransactedA() override final
		{

		}

		void OnMoveFileTransactedW() override final
		{

		}

		void OnMoveFileWithProgressA() override final
		{

		}

		void OnMoveFileWithProgressW() override final
		{

		}

		void OnOpenEncryptedFileRawA() override final
		{

		}

		void OnOpenEncryptedFileRawW() override final
		{

		}

		void OnOpenFile() override final
		{

		}

		void OnOpenFileById() override final
		{

		}

		void OnReadEncryptedFileRaw() override final
		{

		}

		void OnRemoveDirectoryTransactedA() override final
		{

		}

		void OnRemoveDirectoryTransactedW() override final
		{

		}

		void OnReOpenFile() override final
		{

		}

		void OnReplaceFileA() override final
		{

		}

		void OnReplaceFileW() override final
		{

		}

		void OnSetCurrentDirectoryA() override final
		{

		}

		void OnSetCurrentDirectoryW() override final
		{

		}

		void OnSetDllDirectoryA() override final
		{

		}

		void OnSetDllDirectoryW() override final
		{

		}

		void OnSetFileAttributesTransactedA() override final
		{

		}

		void OnSetFileAttributesTransactedW() override final
		{

		}

		void OnSetFileBandwidthReservation() override final
		{

		}

		void OnSetFileCompletionNotificationModes() override final
		{

		}

		void OnSetFileSecurityA() override final
		{

		}

		void OnSetFileShortNameA() override final
		{

		}

		void OnSetFileShortNameW() override final
		{

		}

		void OnSetSearchPathMode() override final
		{

		}

		void OnWriteEncryptedFileRaw() override final
		{

		}

	private:
		std::set<std::string> m_input;
		std::set<std::string> m_output;
	};
}
